#include <HomersDashboard/pages/2023/grabber_page.h>

#include <_2023_cone_none_png.h>
#include <_2023_cone_png.h>
#include <_2023_cube_none_png.h>
#include <_2023_cube_png.h>
#include <_2023_gp_none_png.h>

using namespace y2023;

GrabberPage::GrabberPage(NTHandler& nt_handler)
  : m_nt_handler(nt_handler),
    m_cube_tex(_2023_cube_png, _2023_cube_png_size),
    m_cone_tex(_2023_cone_png, _2023_cone_png_size),
    m_cube_none_tex(_2023_cube_none_png, _2023_cube_none_png_size),
    m_cone_none_tex(_2023_cone_none_png, _2023_cone_none_png_size),
    m_gp_none_tex(_2023_gp_none_png, _2023_gp_none_png_size) {}

void GrabberPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
  if (!ImGui::Begin(name(), running,
                    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoScrollWithMouse)) {
    ImGui::End();
    return;
  }

  const double grabber_pos = m_nt_handler.smart_dashboard()->GetNumber(
      "thunderdashboard_grabber_position", -1.0);

  const double gamepiece = m_nt_handler.smart_dashboard()->GetNumber(
      "thunderdashboard_gamepiece", -1.0);

  const Texture* tex = &m_gp_none_tex;

  // Cube.
  if (gamepiece == 0) {
    tex = &m_cube_tex;
  }
  // Cone.
  else if (gamepiece == 1) {
    tex = &m_cone_tex;
  }
  // None.
  else {
    // Grayed-out cube.
    if (grabber_pos == 0) {
      tex = &m_cube_none_tex;
    }
    // Grayed-out cone.
    else if (grabber_pos == 1) {
      tex = &m_cone_none_tex;
    }
  }

  ImVec2 avail(ImGui::GetContentRegionAvail());

  // Square image.
  const float tex_dim = avail.x < avail.y ? avail.x : avail.y;

  ImGui::Image(tex->id(), ImVec2(tex_dim, tex_dim));

  ImGui::End();
}

