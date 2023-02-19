#include <HomersDashboard/pages/2023/grabber.h>
#include <HomersDashboard/utils.h>
#include <imgui_internal.h>
#include <ThunderDashboard/nt_handler.h>

#include <_2023_cube_png.h>
#include <_2023_cone_png.h>
#include <_2023_cube_none_png.h>
#include <_2023_cone_none_png.h>
#include <_2023_gp_none_png.h>

#define COL_WIDTH 100

using namespace y2023;

GrabberPage::GrabberPage() = default;

GrabberPage::~GrabberPage() = default;

void GrabberPage::init() {
  int width, height, nr_channels;
  cube_tex = Utils::generate_texture_from_memory(_2023_cube_png, _2023_cube_png_size, &width, &height, &nr_channels);
  cone_tex = Utils::generate_texture_from_memory(_2023_cone_png, _2023_cone_png_size, &width, &height, &nr_channels);
  cube_none_tex = Utils::generate_texture_from_memory(_2023_cube_none_png, _2023_cube_none_png_size, &width, &height, &nr_channels);
  cone_none_tex = Utils::generate_texture_from_memory(_2023_cone_none_png, _2023_cone_none_png_size, &width, &height, &nr_channels);
  gp_none_tex = Utils::generate_texture_from_memory(_2023_gp_none_png, _2023_gp_none_png_size, &width, &height, &nr_channels);
}

void GrabberPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("2023 Grabber", running, 
                    ImGuiWindowFlags_NoCollapse
                  | ImGuiWindowFlags_NoScrollbar
                  | ImGuiWindowFlags_NoScrollWithMouse)) {
    ImGui::End();
    return;
  }
  
  focused = ImGui::IsWindowFocused();

  double grabber_pos = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_grabber_position", -1.0);

  double gamepiece = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_gamepiece", -1.0);

  unsigned int tex = gp_none_tex;

  if (gamepiece == 0) { // cube
    tex = cube_tex;
  }
  else if (gamepiece == 1) { // cone 
    tex = cone_tex;
  }
  else { // none
    if (grabber_pos == 0) {
      tex = cube_none_tex;
    }
    else if (grabber_pos == 1) {
      tex = cone_none_tex;
    }
  }

  ImVec2 avail(ImGui::GetContentRegionAvail());
  // Square image.
  float tex_dim = avail.x < avail.y ? avail.x : avail.y;

  ImGui::Image(reinterpret_cast<void*>(tex), ImVec2(tex_dim, tex_dim));

  ImGui::End();
}

GrabberPage GrabberPage::instance;
