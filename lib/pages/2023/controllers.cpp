#include <HomersDashboard/pages/2023/controllers.h>
#include <HomersDashboard/utils.h>
#include <imgui_internal.h>
#include <ThunderDashboard/nt_handler.h>

#include <_2023_controller_driver_png.h>
#include <_2023_controller_aux_png.h>
#include <_2023_controller_aux_manual_png.h>

#define COL_WIDTH 100

using namespace y2023;

ControllerPage::ControllerPage() = default;

ControllerPage::~ControllerPage() = default;

void ControllerPage::init() {
  int width, height, nr_channels;
  driver_tex = Utils::generate_texture_from_memory(_2023_controller_driver_png, _2023_controller_driver_png_size, &width, &height, &nr_channels);
  aux_tex = Utils::generate_texture_from_memory(_2023_controller_aux_png, _2023_controller_aux_png_size, &width, &height, &nr_channels);
  aux_manual_tex = Utils::generate_texture_from_memory(_2023_controller_aux_manual_png, _2023_controller_aux_manual_png_size, &width, &height, &nr_channels);

  aspect_ratio = static_cast<double>(width) / static_cast<double>(height * 3);
}

void ControllerPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("2023 Controller Reference", running, 
                    ImGuiWindowFlags_NoCollapse
                  | ImGuiWindowFlags_NoScrollbar
                  | ImGuiWindowFlags_NoScrollWithMouse)) {
    ImGui::End();
    return;
  }
  
  focused = ImGui::IsWindowFocused();

  ImVec2 win_size(ImGui::GetContentRegionAvail());

  float dim_x(win_size.x),
        dim_y(win_size.y);

  // Fit within the window size while maintaining aspect ratio.
  if ((dim_x / dim_y) > aspect_ratio) {
    dim_x = dim_y * aspect_ratio;
  }
  else {
    dim_y = dim_x / aspect_ratio;
  }

  ImGui::Image(reinterpret_cast<void*>(driver_tex), ImVec2(dim_x, dim_y));
  ImGui::Image(reinterpret_cast<void*>(aux_tex), ImVec2(dim_x, dim_y));
  ImGui::Image(reinterpret_cast<void*>(aux_manual_tex), ImVec2(dim_x, dim_y));

  ImGui::End();
}

ControllerPage ControllerPage::instance;
