#include <HomersDashboard/pages/2022/ball_count_page.h>

#include <_2022_cargo_blue_png.h>
#include <_2022_cargo_none_png.h>
#include <_2022_cargo_red_png.h>

using namespace y2022;

BallCountPage::BallCountPage(NTHandler& nt_handler)
  : m_nt_handler(nt_handler),
    m_no_cargo_tex(_2022_cargo_none_png, _2022_cargo_none_png_size),
    m_red_cargo_tex(_2022_cargo_red_png, _2022_cargo_red_png_size),
    m_blue_cargo_tex(_2022_cargo_blue_png, _2022_cargo_blue_png_size) {}

void BallCountPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
  if (!ImGui::Begin(name(), running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }

  const double ball_count = m_nt_handler.smart_dashboard()->GetNumber(
      "thunderdashboard_ballcount", 0.0);

  const NTHandler::Alliance alliance = m_nt_handler.get_alliance();

  const Texture* cargo_tex = alliance == NTHandler::Alliance::BLUE
                                 ? &m_blue_cargo_tex
                                 : &m_red_cargo_tex;

  const Texture* tex0 = &m_no_cargo_tex;
  const Texture* tex1 = &m_no_cargo_tex;

  if (ball_count >= 1) tex0 = cargo_tex;
  if (ball_count >= 2) tex1 = cargo_tex;

  ImVec2 avail(ImGui::GetContentRegionAvail());

  // Square image.
  const float tex_dim = avail.x / 2.0f;

  ImGui::Image(tex0->id(), ImVec2(tex_dim, tex_dim));
  ImGui::SameLine();
  ImGui::Image(tex1->id(), ImVec2(tex_dim, tex_dim));

  ImGui::Text("Ball Count: %d", static_cast<int>(ball_count));

  ImGui::End();
}

