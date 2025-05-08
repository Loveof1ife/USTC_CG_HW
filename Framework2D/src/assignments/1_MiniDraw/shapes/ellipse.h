#pragma once

#include "common/debug_utils.h"
#include "shape.h"

namespace USTC_CG
{
class Ellipse : public Shape
{
   public:
    Ellipse() = default;

    // Constructor to initialize an ellipse with center and radius
    Ellipse(
        float center_x,
        float center_y,
        float major_axis_x,
        float major_axis_y,
        float ratio = 0.5f,
        float rotation = 0.f)
        : center_x_(center_x),
          center_y_(center_y),
          major_axis_x_(major_axis_x),
          major_axis_y_(major_axis_y),
          ratio_(ratio),
          rotation_(rotation)
    {
        compute_radius();
    }

    virtual ~Ellipse() = default;

    // Overrides draw function to implement ellipse-specific drawing logic
    void draw(const Config& config) const override;

    // Overrides Shape's update function to adjust the end point during
    // interaction
    void update(float x, float y) override;

   private:
    void compute_radius();

   private:
    float center_x_, center_y_, major_axis_x_, major_axis_y_;
    float rotation_ = 0.f, ratio_ = 0.5;
    float radius_x_ = 0.f, radius_y_ = 0.f;
};

}  // namespace USTC_CG