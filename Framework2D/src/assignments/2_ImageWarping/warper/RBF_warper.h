#pragma once

#include <Eigen/Dense>
#include <algorithm>
#include <cmath>
#include <vector>

#include "warper.h"

namespace USTC_CG
{
class RBFWarper : public Warper
{
   public:
    RBFWarper() = default;
    RBFWarper(int width, int height) : Warper(width, height) { };
    ~RBFWarper() override = default;

    std::pair<int, int> warp(int x, int y) override
    {
        if (transforms_dirty_)
        {
            update_transforms();
        }
        return rbf_warping(x, y, width_, height_);
    }

    void set_points(
        const std::vector<ImVec2>& start_points,
        const std::vector<ImVec2>& end_points)
    {
        start_points_ = start_points;
        end_points_ = end_points;
        n_ = start_points_.size();
        transforms_dirty_ = true;
    }

   private:
    void update_transforms();
    std::pair<int, int> rbf_warping(int x, int y, int width, int height);
    float distance(const ImVec2& p1, const ImVec2& p2) const;

   private:
    std::vector<ImVec2> start_points_;
    std::vector<ImVec2> end_points_;
    Eigen::MatrixXf alpha_;  // RBF weights (n x 2)
    Eigen::Matrix2f A_;      // Affine matrix (2x2)
    Eigen::Vector2f b_;      // Affine translation (2x1)
    std::vector<float> r_;   // Support radii for each point
    bool transforms_dirty_ = true;
    size_t n_ = 0;
    int width_ = 0;
    int height_ = 0;
};
}  // namespace USTC_CG