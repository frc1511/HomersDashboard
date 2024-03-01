#include <HomersDashboard/pages/2023/controllers_page.h>

#include <HomersDashboard/opengl_utils.h>

#include <_2023_controller_aux_manual_png.h>
#include <_2023_controller_aux_png.h>
#include <_2023_controller_driver_png.h>

using namespace y2023;

ControllerPage::ControllerPage() {
  int width, height, nr_channels;
  m_driver_tex = generate_texture_from_memory(_2023_controller_driver_png,
                                              _2023_controller_driver_png_size,
                                              &width, &height, &nr_channels);
  m_aux_tex = generate_texture_from_memory(_2023_controller_aux_png,
                                           _2023_controller_aux_png_size,
                                           &width, &height, &nr_channels);
  m_aux_manual_tex = generate_texture_from_memory(
      _2023_controller_aux_manual_png, _2023_controller_aux_manual_png_size,
      &width, &height, &nr_channels);

  m_aspect_ratio = static_cast<double>(width) / static_cast<double>(height);
}

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

  ImGui::Image(reinterpret_cast<void*>(m_driver_tex), ImVec2(dim_x, dim_y));
  ImGui::Image(reinterpret_cast<void*>(m_aux_tex), ImVec2(dim_x, dim_y));
  ImGui::Image(reinterpret_cast<void*>(m_aux_manual_tex), ImVec2(dim_x, dim_y));

  ImGui::End();
}

