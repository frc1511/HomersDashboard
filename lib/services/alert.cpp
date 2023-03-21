#include <HomersDashboard/services/alert.h>
#include <imgui_internal.h>
#include <ThunderDashboard/font_manager.h>

#define COL_WIDTH 100

AlertService::AlertService() = default;

AlertService::~AlertService() = default;

void AlertService::init() { }

void AlertService::process() {
  using namespace std::chrono_literals;

  bool needs_cal = frc1511::NTHandler::get()->get_smart_dashboard()->GetBoolean("thunderdashboard_gyro", false);
  bool lift_broken_kinda = frc1511::NTHandler::get()->get_smart_dashboard()->GetBoolean("thunderdashboard_lift_broken_kinda", false);
  bool lift_broken_a_lot = frc1511::NTHandler::get()->get_smart_dashboard()->GetBoolean("thunderdashboard_lift_broken_a_lot", false);

  std::chrono::steady_clock::time_point now = std::chrono::high_resolution_clock::now();

  if (frc1511::NTHandler::get()->get_instance().IsConnected() && needs_cal || lift_broken_kinda || lift_broken_a_lot) {
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
    ImVec4* col = nullptr;
    if (lift_broken_a_lot) {
      col = &lift_broken_a_lot_bg;
    }
    else if (lift_broken_kinda) {
      col = &lift_broken_kinda_bg;
    }
    else if (needs_cal) {
      col = &gyro_bg;
    }
    if (!col) return;

    style.Colors[ImGuiCol_WindowBg] = *col;
    style.Colors[ImGuiCol_PopupBg] = *col;
    style.Colors[ImGuiCol_ChildBg] = *col;
    style.Colors[ImGuiCol_TableRowBg] = *col;
    style.Colors[ImGuiCol_TableRowBgAlt] = *col;
  }
  else {
    style.Colors[ImGuiCol_WindowBg] = bg_normal;
    style.Colors[ImGuiCol_PopupBg] = bg_normal;
    style.Colors[ImGuiCol_ChildBg] = bg_normal;
    style.Colors[ImGuiCol_TableRowBg] = bg_normal;
    style.Colors[ImGuiCol_TableRowBgAlt] = bg_normal;
  }
}

AlertService AlertService::instance;
