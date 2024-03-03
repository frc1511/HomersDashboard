#include <HomersDashboard/services/alert_service.h>

static const ImVec4 COLOR_NORMAL {0.05f, 0.07f, 0.09f, 1.00f};
static const ImVec4 COLOR_YELLOW {1.00f, 1.00f, 0.00f, 1.00f}; // Yellow
static const ImVec4 COLOR_ORANGE {1.00f, 0.34f, 0.00f, 1.00f}; // Orange
static const ImVec4 COLOR_RED {0.93f, 0.11f, 0.14f, 0.65f};    // Red

void AlertService::init() {}

void AlertService::process() {
  using namespace std::chrono_literals;

  const bool red = m_nt_handler.smart_dashboard()->GetBoolean(
                       "thunderdashboard_gyro", false) ||
                   m_nt_handler.smart_dashboard()->GetBoolean(
                       "thunderdashboard_alert_red", false);
  const bool yellow = m_nt_handler.smart_dashboard()->GetBoolean(
                          "thunderdashboard_lift_broken_kinda", false) ||
                      m_nt_handler.smart_dashboard()->GetBoolean(
                          "thunderdashboard_alert_yellow", false);
  const bool orange = m_nt_handler.smart_dashboard()->GetBoolean(
                          "thunderdashboard_lift_broken_a_lot", false) ||
                      m_nt_handler.smart_dashboard()->GetBoolean(
                          "thunderdashboard_alert_orange", false);

  const auto now = std::chrono::steady_clock::now();

  if (m_nt_handler.instance().IsConnected() && (red || yellow || orange)) {
    const size_t dur_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                              now - m_blink_clock)
                              .count();
    if (dur_ms >= 1000) {
      m_blink = !m_blink;
      m_blink_clock = now;
    }
  } else {
    m_blink = false;
    m_blink_clock = now;
  }

  ImGuiStyle& style = ImGui::GetStyle();

  const ImVec4* col = &COLOR_NORMAL;
  if (m_blink) {
    if (red) {
      col = &COLOR_RED;
    } else if (orange) {
      col = &COLOR_ORANGE;
    } else if (yellow) {
      col = &COLOR_YELLOW;
    }
  }

  style.Colors[ImGuiCol_WindowBg] = style.Colors[ImGuiCol_PopupBg] =
      style.Colors[ImGuiCol_ChildBg] = style.Colors[ImGuiCol_TableRowBg] =
          style.Colors[ImGuiCol_TableRowBgAlt] = *col;
}
