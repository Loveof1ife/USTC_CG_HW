#pragma once
#include "common/image.h"
namespace USTC_CG
{
class SeamlessClone
{
   public:
    SeamlessClone(
        std::shared_ptr<Image> src_image,
        std::shared_ptr<Image> tar_image,
        std::shared_ptr<Image> src_selected_mask)
        : src_image_(src_image),
          tar_image_(tar_image),
          src_selected_mask_(src_selected_mask)
    {
    }

    std::shared_ptr<USTC_CG::Image> solve();  // solve the possion equation
    void set_offset(
        int mouse_position_x,
        int mouse_position_y,
        int selected_region_position_x,
        int selected_region_position_y);

   private:
    void set_src_image_cache(
        const int src_selected_mask_width = 0,
        const int src_selected_mask_height = 0) { };

   private:
    std::shared_ptr<Image> src_image_ = nullptr;
    std::shared_ptr<Image> tar_image_ = nullptr;
    std::shared_ptr<Image> src_selected_mask_ = nullptr;
    std::vector<std::array<double, 3>> src_pixel_cache_{};
    int offset_x_ = 0, offset_y_ = 0;
};

}  // namespace USTC_CG