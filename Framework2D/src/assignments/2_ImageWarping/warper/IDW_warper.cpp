#include "IDW_warper.h"

namespace USTC_CG
{

float calc_distance(const ImVec2& a, const ImVec2& b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return (dx * dx + dy * dy);
}

void IDWWarper::update_transforms()
{
    const size_t n_ = start_points_.size();
    transforms_.resize(n);
    std::vector<std::vector<float>> inv_dist_sq(n, std::vector<float>(n));
    for (size_t i = 0; i < n; ++i)
    {
        inv_dist_sq[i][i] = 1.0f / 1e-10f;

        for (size_t j = i + 1; j < n; ++j)
        {
            float dist = calc_distance(start_points_[i], start_points_[j]);
            float inv_dist = 1.0f / (dist + 1e-10f);
            inv_dist_sq[i][j] = inv_dist_sq[j][i] = inv_dist;
        }
    }

#pragma omp parallel for
    for (int i = 0; i < static_cast<int>(n); ++i)
    {
        Eigen::Matrix2f A = Eigen::Matrix2f::Zero();
        Eigen::Matrix2f B = Eigen::Matrix2f::Zero();

        for (size_t j = 0; j < n; ++j)
        {
            if (j == i)
                continue;

            const float weight = inv_dist_sq[i][j];
            const Eigen::Vector2f p_diff(
                start_points_[j].x - start_points_[i].x,
                start_points_[j].y - start_points_[i].y);
            const Eigen::Vector2f q_diff(
                end_points_[j].x - end_points_[i].x,
                end_points_[j].y - end_points_[i].y);

            A.noalias() += weight * p_diff * p_diff.transpose();
            B.noalias() += weight * q_diff * p_diff.transpose();
        }

        A += Eigen::Matrix2f::Identity() * 1e-10f;
        transforms_[i] = B * A.ldlt().solve(Eigen::Matrix2f::Identity());
    }
    transforms_dirty_ = false;
}
std::pair<int, int> IDWWarper::idw_warping(int x, int y, int width, int height)
{
    if (start_points_.empty())
        return { x, y };

    if (transforms_dirty_)
        update_transforms();

    Eigen::Vector2f result = Eigen::Vector2f::Zero();
    for (int i = 0; i < static_cast<int>(start_points_.size()); i++)
    {
        float sigma_i =
            1.0f / (1e-10f + calc_distance(ImVec2(x, y), start_points_[i]));
        float sum_sigma_j = 0.0f;
        for (int j = 0; j < static_cast<int>(n); j++)
        {
            sum_sigma_j +=
                1.0f / (1e-10f + calc_distance(ImVec2(x, y), start_points_[j]));
        }
        Eigen::Vector2f p(x, y);
        result += (sigma_i / sum_sigma_j) * (transforms_[i] * p);
    }

    return { static_cast<int>(result.x()), static_cast<int>(result.y()) };
}
std::pair<int, int> IDWWarper::warp(int x, int y)
{
    return idw_warping(x, y, width_, height_);
}
}  // namespace USTC_CG