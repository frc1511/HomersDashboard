#include <HomersDashboard/pages/match_timer.h>
#include <imgui_internal.h>
#include <ThunderDashboard/font_manager.h>

#define COL_WIDTH 100

MatchTimerPage::MatchTimerPage() = default;

MatchTimerPage::~MatchTimerPage() = default;

void MatchTimerPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(130, 95), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("Match Timer", running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }
  
  focused = ImGui::IsWindowFocused();

  ImGui::PushFont(frc1511::FontManager::get()->big);
  
  double match_time = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_match_remaining", 911.0);

  if (match_time < 0) {
    match_time = 911.0;
  }

  int minutes = static_cast<int>(std::floor(match_time / 60.0));
  int seconds = static_cast<int>(match_time) % 60;

  bool zero = false;
  if (seconds < 10) {
    zero = true;
  }

  ImGui::Text("%d:%s%d", minutes, zero ? "0" : "", seconds);

  ImGui::PopFont();
  
  ImGui::End();
}

MatchTimerPage MatchTimerPage::instance;
