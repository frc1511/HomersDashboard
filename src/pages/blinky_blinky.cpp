#include <pages/blinky_blinky.h>
#include <imgui_internal.h>

#include <networktables/EntryListenerFlags.h>
#include <networktables/NetworkTableInstance.h>

#define COL_WIDTH 100

BlinkyBlinkyPage::BlinkyBlinkyPage()
: sd_table(nt::NetworkTableInstance::GetDefault().GetTable("SmartDashboard")) { }

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
    sd_table->PutNumber("LED_Mode", led_mode);
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
      sd_table->PutNumber("LED_Custom_Color_R", custom_color[0]);
      sd_table->PutNumber("LED_Custom_Color_G", custom_color[1]);
      sd_table->PutNumber("LED_Custom_Color_B", custom_color[2]);
    }

    ImGui::Columns(1);
  }
  
  ImGui::End();
}

BlinkyBlinkyPage BlinkyBlinkyPage::instance;
