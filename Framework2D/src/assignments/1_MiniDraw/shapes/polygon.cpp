#include "polygon.h"

#include "imgui.h"
namespace USTC_CG
{
void Polygon::add_control_point(float x, float y)
{
    x_list_.push_back(x);
    y_list_.push_back(y);
}
void Polygon::update(float x, float y)
{
    x_list_.back() = x;
    y_list_.back() = y;
}
void Polygon::compete_polygon()
{
    is_completed_ = true;
}
void Polygon::draw(const Config& config) const
{
    if (x_list_.size() < 2)
        return;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    for (size_t i = 0; i < x_list_.size() - 1; ++i)
    {
        draw_list->AddLine(
            ImVec2(config.bias[0] + x_list_[i], config.bias[1] + y_list_[i]),
            ImVec2(
                config.bias[0] + x_list_[i + 1],
                config.bias[1] + y_list_[i + 1]),
            IM_COL32(
                config.line_color[0],
                config.line_color[1],
                config.line_color[2],
                config.line_color[3]),
            config.line_thickness);
    }
    if (is_completed_)
    {
        draw_list->AddLine(
            ImVec2(
                config.bias[0] + x_list_.back(),
                config.bias[1] + y_list_.back()),
            ImVec2(config.bias[0] + x_list_[0], config.bias[1] + y_list_[0]),
            IM_COL32(
                config.line_color[0],
                config.line_color[1],
                config.line_color[2],
                config.line_color[3]),
            config.line_thickness);
    }
}
}  // namespace USTC_CG
