// HW2_TODO: Implement the IDWWarper class
#pragma once

#include <Eigen/Dense>

#include "warper.h"
namespace USTC_CG
{
class IDWWarper : public Warper
{
   public:
    IDWWarper() = default;
    IDWWarper(int width, int height) : Warper(width, height) { };
    ~IDWWarper() override = default;

    std::pair<int, int> warp(int x, int y) override;

    void set_points(
        const std::vector<ImVec2>& start_points,
        const std::vector<ImVec2>& end_points)
    {
        start_points_ = start_points;
        end_points_ = end_points;
        n = start_points_.size();
    }

   private:
    void update_transforms();
    std::pair<int, int> idw_warping(int x, int y, int width, int height);

   private:
    std::vector<ImVec2> start_points_;
    std::vector<ImVec2> end_points_;
    std::vector<Eigen::Matrix2f> transforms_;
    bool transforms_dirty_ = true;
    size_t n = 0;
};
}  // namespace USTC_CG