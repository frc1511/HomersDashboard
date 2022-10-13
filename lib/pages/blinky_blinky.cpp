#include <HomersDashboard/pages/blinky_blinky.h>
#include <imgui_internal.h>

#include <networktables/EntryListenerFlags.h>
#include <networktables/NetworkTableInstance.h>

#define COL_WIDTH 100

BlinkyBlinkyPage::BlinkyBlinkyPage() = default;

BlinkyBlinkyPage::~BlinkyBlinkyPage() = default;

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
    frc1511::NTHandler::get()->get_smart_dashboard()->PutNumber("LED_Mode", led_mode);
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
  frc1511::NTHandler::get()->get_smart_dashboard()->PutNumber("LED_Mode", mode);
}

void BlinkyBlinkyPage::set_custom_r(float r) {
  custom_color[0] = r;
  frc1511::NTHandler::get()->set_double("LED_Custom_Color_R", r);
}

void BlinkyBlinkyPage::set_custom_g(float g) {
  custom_color[1] = g;
  frc1511::NTHandler::get()->set_double("LED_Custom_Color_G", g);
}

void BlinkyBlinkyPage::set_custom_b(float b) {
  custom_color[2] = b;
  frc1511::NTHandler::get()->set_double("LED_Custom_Color_B", b);
}

BlinkyBlinkyPage BlinkyBlinkyPage::instance;
