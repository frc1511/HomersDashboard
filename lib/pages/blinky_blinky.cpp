#include <HomersDashboard/pages/blinky_blinky.h>
#include <imgui_internal.h>

#define COL_WIDTH 100

BlinkyBlinkyPage::BlinkyBlinkyPage() = default;

BlinkyBlinkyPage::~BlinkyBlinkyPage() = default;

void BlinkyBlinkyPage::apply_save_data(const SaveData& save_data) {
  set_custom_r(std::atof(save_data.at("Custom_R").c_str()));
  set_custom_g(std::atof(save_data.at("Custom_G").c_str()));
  set_custom_b(std::atof(save_data.at("Custom_B").c_str()));
  set_led_mode(std::atoi(save_data.at("LED_Mode").c_str()));
}

frc1511::Page::SaveData BlinkyBlinkyPage::get_save_data() const {
  return {
    { "Custom_R", std::to_string(custom_color[0]) },
    { "Custom_G", std::to_string(custom_color[1]) },
    { "Custom_B", std::to_string(custom_color[2]) },
    { "LED_Mode", std::to_string(led_mode) }
  };
}

void BlinkyBlinkyPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("Blinky Blinky", running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }
  
  focused = ImGui::IsWindowFocused();
  
  ImGui::PushID("LED Mode");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("LED Mode");
  ImGui::NextColumn();

  const char* led_modes = "Robot State\0Alliance\0Custom\0Off\0";

  if (ImGui::Combo("##LED Mode", &led_mode, led_modes, 4)) {
    set_led_mode(led_mode);
  }
  
  ImGui::Columns(1);
  ImGui::PopID();

  if (led_mode == 2) {
    ImGui::Separator();

    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, COL_WIDTH);
    ImGui::Text("Custom Color");
    ImGui::NextColumn();

    if (ImGui::ColorEdit3("##Custom Color", custom_color)) {
      set_custom_r(custom_color[0]);
      set_custom_g(custom_color[1]);
      set_custom_b(custom_color[2]);
    }

    ImGui::Columns(1);
  }
  
  ImGui::End();
}

void BlinkyBlinkyPage::set_led_mode(int mode) {
  led_mode = mode;
  frc1511::NTHandler::get()->set_double("thunderdashboard_led_mode", mode);
}

void BlinkyBlinkyPage::set_custom_r(float r) {
  custom_color[0] = r;
  frc1511::NTHandler::get()->set_double("thunderdashboard_led_custom_r", r);
}

void BlinkyBlinkyPage::set_custom_g(float g) {
  custom_color[1] = g;
  frc1511::NTHandler::get()->set_double("thunderdashboard_led_custom_g", g);
}

void BlinkyBlinkyPage::set_custom_b(float b) {
  custom_color[2] = b;
  frc1511::NTHandler::get()->set_double("thunderdashboard_led_custom_b", b);
}

BlinkyBlinkyPage BlinkyBlinkyPage::instance;
