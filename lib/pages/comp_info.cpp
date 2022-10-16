#include <HomersDashboard/pages/comp_info.h>
#include <imgui_internal.h>
#include <ThunderDashboard/font_manager.h>

#define COL_WIDTH 100

CompInfoPage::CompInfoPage() = default;

CompInfoPage::~CompInfoPage() = default;

void CompInfoPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(420, 1690), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("Competition Info", running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }
  
  focused = ImGui::IsWindowFocused();

  ImVec4 color;
  const char* state_str;
  switch (frc1511::NTHandler::get()->get_match_mode()) {
    case frc1511::NTHandler::MatchMode::DISABLED:
      color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red
      state_str = "Disabled";
      break;
    case frc1511::NTHandler::MatchMode::AUTO:
      color = ImVec4(0.0f, 0.0f, 1.0f, 1.0f); // Blue
      state_str = "Autonomous";
      break;
    case frc1511::NTHandler::MatchMode::TELEOP:
      color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Green
      state_str = "Teleop";
      break;
    case frc1511::NTHandler::MatchMode::TEST:
      color = ImVec4(1.0f, 0.0f, 1.0f, 1.0f); // Purple
      state_str = "Test";
      break;
    case frc1511::NTHandler::MatchMode::ESTOPPED:
      color = ImVec4(0.0f, 1.0f, 1.0f, 1.0f); // Yellow
      state_str = "EStopped";
      break;
  }

  ImGui::PushFont(frc1511::FontManager::get()->big);
  ImGui::PushStyleColor(ImGuiCol_Text, color);
  ImGui::TextUnformatted(state_str);
  ImGui::PopStyleColor();
  ImGui::PopFont();

  ImGui::Text("Event Name: %s", frc1511::NTHandler::get()->get_event_name().c_str());
  ImGui::Text("Game Message: %s", frc1511::NTHandler::get()->get_game_msg().c_str());

  const char* type_str;
  switch (frc1511::NTHandler::get()->get_match_type()) {
    case frc1511::NTHandler::MatchType::NONE:
      type_str = "None";
      break;
    case frc1511::NTHandler::MatchType::PRACTICE:
      type_str = "Practice";
      break;
    case frc1511::NTHandler::MatchType::QUAL:
      type_str = "Qualification";
      break;
    case frc1511::NTHandler::MatchType::ELIM:
      type_str = "Elimination";
      break;
  }

  ImGui::Text("Match Type: %s", type_str);
  ImGui::Text("Match Number: %zu", frc1511::NTHandler::get()->get_match_number());
  ImGui::Text("Replay Number: %zu", frc1511::NTHandler::get()->get_replay_number());

  ImGui::Text("Station Number: %zu", frc1511::NTHandler::get()->get_station_number());

  const char* alliance_str = "Red";
  if (frc1511::NTHandler::get()->get_alliance() == frc1511::NTHandler::Alliance::BLUE) {
    alliance_str = "Blue";
  }

  ImGui::Text("Alliance: %s", alliance_str);

  ImGui::End();
}

CompInfoPage CompInfoPage::instance;
