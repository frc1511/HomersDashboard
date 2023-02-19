#include <HomersDashboard/pages/2022/ball_count.h>
#include <HomersDashboard/utils.h>

#include <_2022_cargo_blue_png.h>
#include <_2022_cargo_red_png.h>
#include <_2022_cargo_none_png.h>

#define COL_WIDTH 100

using namespace y2022;

BallCountPage::BallCountPage() = default;

BallCountPage::~BallCountPage() = default;

void BallCountPage::init() {
  int width, height, nr_channels;
  no_cargo_tex = Utils::generate_texture_from_memory(_2022_cargo_none_png, _2022_cargo_none_png_size, &width, &height, &nr_channels);
  red_cargo_tex = Utils::generate_texture_from_memory(_2022_cargo_blue_png, _2022_cargo_blue_png_size, &width, &height, &nr_channels);
  blue_cargo_tex = Utils::generate_texture_from_memory(_2022_cargo_red_png, _2022_cargo_red_png_size, &width, &height, &nr_channels);
}

void BallCountPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("2022 Ball Count", running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }
  
  focused = ImGui::IsWindowFocused();

  double ball_count = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_ballcount", 0.0);
  frc1511::NTHandler::Alliance alliance = frc1511::NTHandler::get()->get_alliance();

  unsigned int tex0 = no_cargo_tex, tex1 = no_cargo_tex;

  auto get_ball_tex = [&]() -> unsigned int {
    return alliance == frc1511::NTHandler::Alliance::BLUE ? blue_cargo_tex : red_cargo_tex;
  };

  if (ball_count >= 1) {
    tex0 = get_ball_tex();
  }
  if (ball_count >= 2) {
    tex1 = get_ball_tex();
  }
  
  ImVec2 avail(ImGui::GetContentRegionAvail());
  // Square image.
  float tex_dim = avail.x / 2.0f;

  ImGui::Image(reinterpret_cast<void*>(tex0), ImVec2(tex_dim, tex_dim));
  ImGui::SameLine();
  ImGui::Image(reinterpret_cast<void*>(tex1), ImVec2(tex_dim, tex_dim));

  ImGui::Text("Ball Count: %d", static_cast<int>(ball_count));

  ImGui::End();
}

BallCountPage BallCountPage::instance;
