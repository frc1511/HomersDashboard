#include <HomersDashboard/gyro.h>
#include <imgui_internal.h>
#include <ThunderDashboard/font_manager.h>

#define COL_WIDTH 100

GyroHandler::GyroHandler() = default;

GyroHandler::~GyroHandler() = default;

void GyroHandler::handle_calibration() {
  using namespace std::chrono_literals;

  bool needs_cal = static_cast<bool>(frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_gyro", 0.0));

  std::chrono::steady_clock::time_point now = std::chrono::high_resolution_clock::now();

  if (frc1511::NTHandler::get()->get_instance().IsConnected() && needs_cal) {
    double dur = std::chrono::duration_cast<std::chrono::milliseconds>(now - blinkClock).count() * 0.001;
    if (dur >= 1.0) {
      blink = !blink;
      blinkClock = now;
    }
  }
  else {
    blink = false;
    blinkClock = now;
  }

  ImGuiStyle& style = ImGui::GetStyle();

  if (blink) {
    style.Colors[ImGuiCol_WindowBg] = bg_blink;
    style.Colors[ImGuiCol_PopupBg] = bg_blink;
    style.Colors[ImGuiCol_ChildBg] = bg_blink;
    style.Colors[ImGuiCol_TableRowBg] = bg_blink;
    style.Colors[ImGuiCol_TableRowBgAlt] = bg_blink;
  }
  else {
    style.Colors[ImGuiCol_WindowBg] = bg_normal;
    style.Colors[ImGuiCol_PopupBg] = bg_normal;
    style.Colors[ImGuiCol_ChildBg] = bg_normal;
    style.Colors[ImGuiCol_TableRowBg] = bg_normal;
    style.Colors[ImGuiCol_TableRowBgAlt] = bg_normal;
  }
}

GyroHandler GyroHandler::instance;
