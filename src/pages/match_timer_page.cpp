#include <HomersDashboard/pages/match_timer_page.h>

void MatchTimerPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(130, 95), ImGuiCond_FirstUseEver);
  if (!ImGui::Begin(name(), running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }

  ImGui::PushFont(m_font_lib.big_font);

  double match_time = m_nt_handler.smart_dashboard()->GetNumber(
      "thunderdashboard_match_remaining", 911.0);

  if (match_time < 0) {
    match_time = 911.0;
  }

  const int minutes = static_cast<int>(std::floor(match_time / 60.0));
  const int seconds = static_cast<int>(match_time) % 60;

  bool zero = false;
  if (seconds < 10) {
    zero = true;
  }

  ImGui::Text("%d:%s%d", minutes, zero ? "0" : "", seconds);

  ImGui::PopFont();

  ImGui::End();
}
