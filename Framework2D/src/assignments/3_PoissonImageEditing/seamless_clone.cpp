#include "seamless_clone.h"

#include <omp.h>

#include <Eigen/IterativeLinearSolvers>
#include <iostream>

#include "common/debug_utils.h"
#include "common/runtime_assert.h"

namespace USTC_CG
{
void SeamlessClone::set_offset(
    int mouse_position_x,
    int mouse_position_y,
    int selected_region_position_x,
    int selected_region_position_y)
{
    offset_x_ = mouse_position_x - selected_region_position_x;
    offset_y_ = mouse_position_y - selected_region_position_y;

    if (DebugUtils::Debugger::level > 0)
    {
        std::cout << "selected region position: (" << selected_region_position_x
                  << "," << selected_region_position_y << ")" << std::endl;
        std::cout << "offset_y_ = " << offset_y_ << std::endl;
        std::cout << "offset_x_ = " << offset_x_ << std::endl;
        std::cout << "offset_y_ = " << offset_y_ << std::endl;
    }
}

std::shared_ptr<USTC_CG::Image> SeamlessClone::solve()
{
    // Step 1: Identify the rectangular region to clone
    int min_x = src_selected_mask_->width(), max_x = 0;
    int min_y = src_selected_mask_->height(), max_y = 0;
    std::unordered_map<int, std::vector<int>> interiors_map;

    for (int y = 0; y < src_selected_mask_->height(); ++y)
    {
        for (int x = 0; x < src_selected_mask_->width(); ++x)
        {
            const auto& mask_pixel = src_selected_mask_->get_pixel(x, y);
            if (static_cast<int>(mask_pixel[0]) > 0)
            {
                if (DebugUtils::Debugger::level > 1)
                {
                    std::cout << "Found a clone region at (" << x << "," << y
                              << ")" << std::endl;
                }
                interiors_map[y].push_back(x);
                min_x = std::min(min_x, x);
                max_x = std::max(max_x, x);
                min_y = std::min(min_y, y);
                max_y = std::max(max_y, y);
            }
        }
    }

    const int interiors_map_height = static_cast<int>(interiors_map.size());
    const int interiors_map_width =
        static_cast<int>(interiors_map.begin()->second.size());

    const int total_pixels =
        static_cast<int>(interiors_map_height * interiors_map_width);
    const int region_width = max_x - min_x + 1;
    const int region_height = max_y - min_y + 1;

    AA_RUNTIME_ASSERT(
        interiors_map_height == region_height &&
        interiors_map_width == region_width);

    if (DebugUtils::Debugger::level > 0)
    {
        for (auto it = interiors_map.begin(); it != interiors_map.end(); ++it)
        {
            if (it->second.size() != interiors_map.begin()->second.size())
                std::cout << "unmatched size" << it->second.size() << std::endl;
        }
    }
    if (total_pixels == 0)
    {
        std::cerr << "No region selected for cloning" << std::endl;
        return nullptr;
    }

    Eigen::SparseMatrix<double> A(total_pixels, total_pixels);

    Eigen::VectorXd Br(total_pixels), Bg(total_pixels), Bb(total_pixels);

    std::cout << " Region width: " << region_width
              << " Region height: " << region_height
              << " Total pixels: " << total_pixels
              << std::endl;  // 应等于 width * height

    // TODO:: Source image pixels Cache for HPC
    const int src_image_width = src_image_->width();
    const int src_image_height = src_image_->height();
    set_src_image_cache(src_image_width, src_image_height);

    // Linear system
    std::vector<Eigen::Triplet<double>> triplet_list;
    triplet_list.reserve(5 * total_pixels);
    for (const auto& [y, x_vec] : interiors_map)
    {
        for (const auto& x : x_vec)
        {
            int tar_x = x + offset_x_;
            int tar_y = y + offset_y_;

            if (tar_x < 0 || tar_x >= tar_image_->width() || tar_y < 0 ||
                tar_y >= tar_image_->height())
            {
                continue;
            }

            int f_star_idx = (y - min_y) * region_width + (x - min_x);
            int f_star_bottom_idx = f_star_idx + region_width,
                f_star_top_idx = f_star_idx - region_width,
                f_star_left_idx = f_star_idx - 1,
                f_star_right_idx = f_star_idx + 1;

            //  boundary condition
            auto is_boundary_pixel = [&](int x, int y) -> bool
            {
                // Current pixel must be interior (mask value > 0)
                if (src_selected_mask_->get_pixel(x, y)[0] == 0)
                    return false;
                // Check 4-neighborhood (top/bottom/left/right)
                constexpr int dx[] = { 0, 0, -1, 1 };
                constexpr int dy[] = { -1, 1, 0, 0 };

                for (int i = 0; i < 4; ++i)
                {
                    int nx = x + dx[i];
                    int ny = y + dy[i];
                    // Skip out-of-bound pixels
                    if (nx < 0 || nx >= src_selected_mask_->width() || ny < 0 ||
                        ny >= src_selected_mask_->height())
                    {
                        continue;
                    }

                    // Found at least one exterior neighbor
                    if (src_selected_mask_->get_pixel(nx, ny)[0] == 0)
                    {
                        return true;
                    }
                }
                return false;
            };

            // Usage in your boundary condition:
            if (is_boundary_pixel(x, y))
            {
                std::cout << "Boundary pixel at"
                          << "(" << x << "," << y << ")" << std::endl;
                const auto& tar_pixel = tar_image_->get_pixel(tar_x, tar_y);
                triplet_list.emplace_back(f_star_idx, f_star_idx, 1.0);
                Br(f_star_idx) = tar_pixel[0];
                Bg(f_star_idx) = tar_pixel[1];
                Bb(f_star_idx) = tar_pixel[2];
            }
            else
            {
                // interior condition
                // (x, y) is the position of the selected region
                // index: x*width + y
                // neighborhood: (x, y-1), (x, y+1), (x-1,y), (x+1,y)
                // index: (x+width + y-1), (x+width + y+1), (x-1+width + y),
                // (x+1+width + y)
                triplet_list.emplace_back(f_star_idx, f_star_idx, 4.0);
                triplet_list.emplace_back(f_star_idx, f_star_top_idx, -1.0);
                triplet_list.emplace_back(f_star_idx, f_star_bottom_idx, -1.0);
                triplet_list.emplace_back(f_star_idx, f_star_left_idx, -1.0);
                triplet_list.emplace_back(f_star_idx, f_star_right_idx, -1.0);

                // Compute B values from source image gradients
                const auto& center = src_image_->get_pixel(x, y);
                const auto& top = src_image_->get_pixel(x, y - 1);
                const auto& bottom = src_image_->get_pixel(x, y + 1);
                const auto& left = src_image_->get_pixel(x - 1, y);
                const auto& right = src_image_->get_pixel(x + 1, y);

                Br(f_star_idx) =
                    4 * center[0] - top[0] - bottom[0] - left[0] - right[0];
                Bg(f_star_idx) =
                    4 * center[1] - top[1] - bottom[1] - left[1] - right[1];
                Bb(f_star_idx) =
                    4 * center[2] - top[2] - bottom[2] - left[2] - right[2];
            }
            if (DebugUtils::Debugger::level > 1)
                std::cout << "linear system setting at (x, y)" << "(" << x
                          << "," << y << ")" << std::endl;
        }
    }
    if (DebugUtils::Debugger::level > 0)
        std::cout << "triplets building finished, size: " << triplet_list.size()
                  << std::endl;
    // Build the sparse matrix
    A.setFromTriplets(triplet_list.begin(), triplet_list.end());

    if (DebugUtils::Debugger::level > 0)
    {
        DebugUtils::debugSparseMatrix(A);
        DebugUtils::checkDiagonalDominance(A);
    }

    // Step 4: Solve with a more robust solver
    Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
    solver.compute(A);

    if (solver.info() == Eigen::Success)
    {
        std::cerr << "Matrix decomposition success" << std::endl;
        std::cout << "Matrix non-zero entries: " << A.nonZeros() << std::endl;
        std::cout << "Matrix size: " << A.rows() << "x" << A.cols()
                  << std::endl;
    }
    Eigen::VectorXd r = solver.solve(Br);
    Eigen::VectorXd g = solver.solve(Bg);
    Eigen::VectorXd b = solver.solve(Bb);
    if (solver.info() == Eigen::Success)
    {
        std::cerr << "Solving success" << std::endl;
        std::cout << "Red channel range: " << r.minCoeff() << " to "
                  << r.maxCoeff() << std::endl;
        std::cout << "Green channel range: " << g.minCoeff() << " to "
                  << g.maxCoeff() << std::endl;
        std::cout << "Blue channel range: " << b.minCoeff() << " to "
                  << b.maxCoeff() << std::endl;
    }
    else
    {
        std::cerr << "Solving failed" << std::endl;
        return nullptr;
    }

    // #pragma omp parallel for
    for (int i = 0; i < total_pixels; ++i)
    {
        int x = i % region_width + min_x;
        int y = i / region_width + min_y;
        int tar_x = x + offset_x_;
        int tar_y = y + offset_y_;

        if (tar_x >= 0 && tar_x < tar_image_->width() && tar_y >= 0 &&
            tar_y < tar_image_->height())
        {
            tar_image_->set_pixel(
                tar_x,
                tar_y,
                { static_cast<unsigned char>(r(i)),
                  static_cast<unsigned char>(g(i)),
                  static_cast<unsigned char>(b(i)) });
        }
    }
    return tar_image_;
}
}  // namespace USTC_CG