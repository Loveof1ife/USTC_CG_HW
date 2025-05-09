// HW2_TODO: Please implement the abstract class Warper
// 1. The Warper class should abstract the **mathematical mapping** involved in
// the warping problem, **independent of image**.
// 2. The Warper class should have a virtual function warp(...) to be called in
// our image warping application.
//    - You should design the inputs and outputs of warp(...) according to the
//    mathematical abstraction discussed in class.
//    - Generally, the warping map should map one input point to another place.
// 3. Subclasses of Warper, IDWWarper and RBFWarper, should implement the
// warp(...) function to perform the actual warping.
#pragma once

#include <memory>
#include <vector>

#include "common/image.h"
#include "imgui.h"
namespace USTC_CG
{
class Warper
{
   public:
    Warper(int width, int height) : width_(width), height_(height) { };
    virtual ~Warper() = default;

    // HW2_TODO: A virtual function warp(...)
    virtual std::pair<int, int> warp(int x, int y) = 0;
    // HW2_TODO: other functions or variables if you need
   protected:
    int width_ = 0, height_ = 0;
};

class FisheyeWarper : public Warper
{
   public:
    FisheyeWarper(int width, int height) : Warper(width, height) { };
    ~FisheyeWarper() override = default;

    std::pair<int, int> warp(int x, int y) override;

   private:
    std::pair<int, int> fisheye_warping(int x, int y, int width, int height);
};

class IDWWarper : public Warper
{
   public:
    IDWWarper(int width, int height) : Warper(width, height) { };
    ~IDWWarper() override = default;

    std::pair<int, int> warp(int x, int y) override;
    void set_points(
        const std::vector<ImVec2>& start_points,
        const std::vector<ImVec2>& end_points)
    {
        start_points_ = start_points;
        end_points_ = end_points;
    }

   private:
    std::pair<int, int> idw_warping(int x, int y, int width, int height);

   private:
    std::vector<ImVec2> start_points_;
    std::vector<ImVec2> end_points_;
};
}  // namespace USTC_CG