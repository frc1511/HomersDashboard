#include <HomersDashboard/pages/2022/shooter_position_page.h>

#include <HomersDashboard/imgui_util.h>

#define COLUMN_WIDTH 50

using namespace y2022;

void ShooterPositionPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(300, 120), ImGuiCond_FirstUseEver);
  if (!ImGui::Begin(name(), running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }

  {
    ImGuiScopedField field("RPM", COLUMN_WIDTH);

    const float rpm_max(m_nt_handler.smart_dashboard()->GetNumber(
        "thunderdashboard_rpm_max", 2600.0));

    const float rpm(
        m_nt_handler.smart_dashboard()->GetNumber("thunderdashboard_rpm", 0.0));

    const float rpm_pct = float_eq(rpm_max, 0.0f) ? 0.f : rpm / rpm_max;

    const std::string buf = std::to_string(rpm);
    ImGui::ProgressBar(rpm_pct, ImVec2(ImGui::GetContentRegionAvail().x, 30),
                       buf.c_str());
  }

  {
    ImGuiScopedField field("Hood", COLUMN_WIDTH);

    const float hood_min(m_nt_handler.smart_dashboard()->GetNumber(
        "thunderdashboard_hood_min", 0.0));

    const float hood_max(m_nt_handler.smart_dashboard()->GetNumber(
        "thunderdashboard_hood_max", 1.0));

    const float hood(m_nt_handler.smart_dashboard()->GetNumber(
        "thunderdashboard_hood", 0.0));

    const float hood_pct = float_eq(hood_max, hood_min)
                               ? 0.f
                               : (hood - hood_min) / (hood_max - hood_min);

    std::string buf = std::to_string(hood - hood_min);
    ImGui::ProgressBar(hood_pct, ImVec2(ImGui::GetContentRegionAvail().x, 30),
                       buf.c_str());
  }

  ImGui::End();
}
