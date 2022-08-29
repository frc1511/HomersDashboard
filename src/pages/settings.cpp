#include <pages/settings.h>

SettingsPage::SettingsPage() = default;

SettingsPage::~SettingsPage() = default;

void SettingsPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_FirstUseEver);
  
  if (!ImGui::Begin("Settings", running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }

  focused = ImGui::IsWindowFocused();
  
  if (ImGui::InputText("Team Number", number_buf, 9, ImGuiInputTextFlags_CharsDecimal)) {
    team_number = std::atol(number_buf);
    nt::NetworkTableInstance::GetDefault().SetServerTeam(team_number);
    std::cout << "Set team number to " << team_number << std::endl;
  }
  
  ImGui::End();
}

void SettingsPage::set_team_number(std::size_t number) {
  team_number = number;
  sprintf(number_buf, "%lu", team_number);
  
  nt::NetworkTableInstance::GetDefault().SetServerTeam(team_number);
}

SettingsPage SettingsPage::instance {};
