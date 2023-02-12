#include <HomersDashboard/pages/settings.h>
#include <ThunderDashboard/nt_handler.h>
#ifdef THUNDER_WINDOWS
#include <HomersDashboard/ps5_contoller_handler.h>
#endif

#define COL_WIDTH 150

SettingsPage::SettingsPage() = default;

SettingsPage::~SettingsPage() = default;

void SettingsPage::apply_save_data(const SaveData& save_data) {
  set_team_number(std::atoi(save_data.at("Team_Number").c_str()));
#ifdef THUNDER_WINDOWS
  int driver_id = std::atoi(save_data.at("PS5_Driver_ID").c_str());
  int aux_id = std::atoi(save_data.at("PS5_Aux_ID").c_str());
  PS5ControllerHandler::get()->set_controller_ids(driver_id, aux_id);
#endif
}

frc1511::Page::SaveData SettingsPage::get_save_data() const {
#ifdef THUNDER_WINDOWS
  auto [driver_id, aux_id] = PS5ControllerHandler::get()->get_controller_ids();
#endif
  return {
    { "Team_Number", std::to_string(team_number) },
#ifdef THUNDER_WINDOWS
    { "PS5_Driver_ID", std::to_string(driver_id) },
    { "PS5_Aux_ID", std::to_string(aux_id) }
#endif
  };
}

void SettingsPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_FirstUseEver);
  
  if (!ImGui::Begin("Settings", running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }

  focused = ImGui::IsWindowFocused();

  ImGui::PushID("Team Number");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("Team Number");
  ImGui::NextColumn();

  if (ImGui::InputText("##Team Number", number_buf, 9, ImGuiInputTextFlags_CharsDecimal)) {
    team_number = std::atol(number_buf);
    frc1511::NTHandler::get()->get_instance().SetServerTeam(team_number); // TODO: Change
    std::cout << "Set team number to " << team_number << std::endl;
  }

  ImGui::Columns(1);
  ImGui::PopID();

#ifdef THUNDER_WINDOWS
  auto [old_driver_id, old_aux_id] = PS5ControllerHandler::get()->get_controller_ids();
  int driver_id = old_driver_id;
  int aux_id = old_aux_id;

  ImGui::PushID("Driver PS5 Controller");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("Driver PS5 Controller");
  ImGui::NextColumn();

  ImGui::InputInt("##Driver Controller ID", &driver_id);

  ImGui::Columns(1);
  ImGui::PopID();

  ImGui::PushID("Test Driver PS5 Controller");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("Test Driver PS5 Controller");
  ImGui::NextColumn();

  bool testing_driver = false;

  ImGui::PushButtonRepeat(true);
  if (ImGui::Button("Test Driver")) {
    testing_driver = true;
  }
  ImGui::PopButtonRepeat();

  PS5ControllerHandler::get()->set_test_driver_controller(testing_driver);

  ImGui::Columns(1);
  ImGui::PopID();

  ImGui::PushID("Aux PS5 Controller");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("Aux PS5 Controller");
  ImGui::NextColumn();

  ImGui::InputInt("##Aux Controller ID", &aux_id);

  ImGui::Columns(1);
  ImGui::PopID();

  ImGui::PushID("Test Aux PS5 Controller");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("Test Aux PS5 Controller");
  ImGui::NextColumn();

  bool testing_aux = false;

  ImGui::PushButtonRepeat(true);
  if (ImGui::Button("Test Aux")) {
    testing_aux = true;
  }
  ImGui::PopButtonRepeat();

  PS5ControllerHandler::get()->set_test_aux_controller(testing_aux);

  ImGui::Columns(1);
  ImGui::PopID();

  if (driver_id != old_driver_id || aux_id != old_aux_id) {
    PS5ControllerHandler::get()->set_controller_ids(driver_id, aux_id);
  }

  ImGui::PushID("Reload PS5 Controllers");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("Reload PS5 Controllers");
  ImGui::NextColumn();

  if (ImGui::Button("Reload")) {
    PS5ControllerHandler::get()->reload_connections();
  }

  ImGui::Columns(1);
  ImGui::PopID();
#endif
  
  ImGui::End();
}

void SettingsPage::set_team_number(std::size_t number) {
  team_number = number;
  sprintf(number_buf, "%lu", team_number);
  
  frc1511::NTHandler::get()->get_instance().SetServerTeam(team_number);
}

SettingsPage SettingsPage::instance {};
