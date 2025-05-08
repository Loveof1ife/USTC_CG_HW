#include "ellipse.h"

#include <imgui.h>

#include <cmath>
#include <iostream>
namespace USTC_CG
{
void Ellipse::draw(const Config& config) const
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    if (DebugUtils::Debugger::level > 0)
    {
        // 硬编码测试参数
        draw_list->AddEllipse(
            ImVec2(200, 200),
            ImVec2(100, 50),
            IM_COL32(255, 0, 0, 255),
            0.0f,
            0,  // 自动分段
            2.0f);
    }
    else
    {
        draw_list->AddEllipse(
            ImVec2(config.bias[0] + center_x_, config.bias[1] + center_y_),
            ImVec2(radius_x_, radius_y_),
            IM_COL32(
                config.line_color[0],
                config.line_color[1],
                config.line_color[2],
                config.line_color[3]),
            rotation_,
            0,
            config.line_thickness);
    }
}
void Ellipse::update(float x, float y)
{
    if (DebugUtils::Debugger::level > 0)
    {
        std::cout << "Ellipse update - new major axis point: (" << x << ", "
                  << y << ")\n";
    }
    major_axis_x_ = x;
    major_axis_y_ = y;
    compute_radius();
}
void Ellipse::compute_radius()
{
    radius_x_ = std::sqrt(
        std::pow(center_x_ - major_axis_x_, 2) +
        std::pow(center_y_ - major_axis_y_, 2));
    radius_y_ = ratio_ * radius_x_;

    if (DebugUtils::Debugger::level > 0)
    {
        std::cout << "Center: (" << center_x_ << ", " << center_y_ << ")\n";
        std::cout << "Radius X: " << radius_x_ << ", Radius Y: " << radius_y_
                  << "\n";
        std::cout << "Rotation: " << rotation_ << " radians\n";
    }
}

}  // namespace USTC_CG
