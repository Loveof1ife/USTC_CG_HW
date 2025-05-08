#pragma once

#include <vector>

#include "common/debug_utils.h"
#include "shape.h"

namespace USTC_CG
{
class Polygon : public Shape
{
   public:
    Polygon() = default;
    Polygon(std::vector<float> x_list, std::vector<float> y_list);
    virtual ~Polygon() = default;

    void draw(const Config& config) const;
    void update(float x, float y) override;
    void add_control_point(float x, float y) override;
    void compete_polygon();

   private:
    std::vector<float> x_list_, y_list_;
    bool is_completed_ = false;
};

}  // namespace USTC_CG
