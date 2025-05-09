#include "warper.h"

#include <iostream>
namespace USTC_CG
{
std::pair<int, int>
FisheyeWarper::fisheye_warping(int x, int y, int width, int height)
{
    float center_x = width / 2.0f;
    float center_y = height / 2.0f;
    float dx = x - center_x;
    float dy = y - center_y;
    float distance = std::sqrt(dx * dx + dy * dy);

    // Simple non-linear transformation r -> r' = f(r)
    float new_distance = std::sqrt(distance) * 10;

    if (distance == 0)
    {
        return { static_cast<int>(center_x), static_cast<int>(center_y) };
    }
    // (x', y')
    float ratio = new_distance / distance;
    int new_x = static_cast<int>(center_x + dx * ratio);
    int new_y = static_cast<int>(center_y + dy * ratio);

    return { new_x, new_y };
}

std::pair<int, int> FisheyeWarper::warp(int x, int y)
{
    return fisheye_warping(x, y, width_, height_);
}

std::pair<int, int> IDWWarper::idw_warping(int x, int y, int width, int height)
{
    size_t n = start_points_.size();
    if (n == 0)
    {
        return { x, y };
    }
    std::vector<Eigen::MatrixXf> transforms(n, Eigen::MatrixXf::Zero(2, 2));

    return { x, y };
}
std::pair<int, int> IDWWarper::warp(int x, int y)
{
    return idw_warping(x, y, width_, height_);
}
}  // namespace USTC_CG