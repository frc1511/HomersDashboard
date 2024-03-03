#include <HomersDashboard/pages/2023/controllers_page.h>

#include <_2023_controller_aux_manual_png.h>
#include <_2023_controller_aux_png.h>
#include <_2023_controller_driver_png.h>

using namespace y2023;

ControllerPage::ControllerPage()
  : m_driver_tex(_2023_controller_driver_png, _2023_controller_driver_png_size),

    m_aux_tex(_2023_controller_aux_png, _2023_controller_aux_png_size),

    m_aux_manual_tex(_2023_controller_aux_manual_png,
                     _2023_controller_aux_manual_png_size),

    m_aspect_ratio(static_cast<float>(m_driver_tex.width()) /
                   m_driver_tex.height()) {}

void ControllerPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
  if (!ImGui::Begin(name(), running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }

  const ImVec2 win_size(ImGui::GetContentRegionAvail());

  float dim_x(win_size.x), dim_y(win_size.y);

  // Fit within the window size while maintaining aspect ratio.
  if ((dim_x / dim_y) > m_aspect_ratio) {
    dim_x = dim_y * m_aspect_ratio;
  } else {
    dim_y = dim_x / m_aspect_ratio;
  }

  ImGui::Image(m_driver_tex.id(), ImVec2(dim_x, dim_y));
  ImGui::Image(m_aux_tex.id(), ImVec2(dim_x, dim_y));
  ImGui::Image(m_aux_manual_tex.id(), ImVec2(dim_x, dim_y));

  ImGui::End();
}

