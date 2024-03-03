#include <HomersDashboard/pages/competition_info_page.h>

void CompetitionInfoPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(260, 217), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin(name(), running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }

  ImVec4 color;
  const char* state_str = "";
  switch (m_nt_handler.get_match_mode()) {
    using enum NTHandler::MatchMode;
  case DISABLED:
    color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red
    state_str = "Disabled";
    break;
  case AUTO:
    color = ImVec4(0.0f, 0.0f, 1.0f, 1.0f); // Blue
    state_str = "Autonomous";
    break;
  case TELEOP:
    color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Green
    state_str = "Teleop";
    break;
  case TEST:
    color = ImVec4(1.0f, 0.0f, 1.0f, 1.0f); // Purple
    state_str = "Test";
    break;
  case ESTOPPED:
    color = ImVec4(0.0f, 1.0f, 1.0f, 1.0f); // Yellow
    state_str = "EStopped";
    break;
  }

  ImGui::PushFont(m_font_lib.big_font);
  ImGui::PushStyleColor(ImGuiCol_Text, color);
  ImGui::TextUnformatted(state_str);
  ImGui::PopStyleColor();
  ImGui::PopFont();

  ImGui::Text("Event Name: %s", m_nt_handler.get_event_name().c_str());
  ImGui::Text("Game Message: %s", m_nt_handler.get_game_msg().c_str());

  const char* type_str;
  switch (m_nt_handler.get_match_type()) {
    using enum NTHandler::MatchType;
  case NONE:
    type_str = "None";
    break;
  case PRACTICE:
    type_str = "Practice";
    break;
  case QUAL:
    type_str = "Qualification";
    break;
  case ELIM:
    type_str = "Elimination";
    break;
  }

  ImGui::Text("Match Type: %s", type_str);
  ImGui::Text("Match Number: %zu", m_nt_handler.get_match_number());
  ImGui::Text("Replay Number: %zu", m_nt_handler.get_replay_number());

  ImGui::Text("Station Number: %zu", m_nt_handler.get_station_number());

  const char* alliance_str = "Red";
  if (m_nt_handler.get_alliance() == NTHandler::Alliance::BLUE) {
    alliance_str = "Blue";
  }

  ImGui::Text("Alliance: %s", alliance_str);

  ImGui::End();
}
