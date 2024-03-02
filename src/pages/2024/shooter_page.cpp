#include <HomersDashboard/pages/2024/shooter_page.h>

#include <HomersDashboard/imgui_util.h>

#define COLUMN_WIDTH 50

using namespace y2024;

void ShooterPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(300, 120), ImGuiCond_FirstUseEver);
  if (!ImGui::Begin(name(), running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }

  const double rpm_max = m_nt_handler.smart_dashboard()->GetNumber(
      "thunderdashboard_rpm_max", 2600.0);

  const double rpm =
      m_nt_handler.smart_dashboard()->GetNumber("thunderdashboard_rpm", 0.0);

  const double rpm_pct = float_eq(rpm_max, 0.0f) ? 0.0f : rpm / rpm_max;

  {
    ImGuiScopedField field("RPM", COLUMN_WIDTH, [rpm]{
      ImGui::Text("%.0f\nRPM", rpm);
    });

    const std::string buf = std::to_string(int(rpm));
    ImGui::ProgressBar(rpm_pct, ImVec2(ImGui::GetContentRegionAvail().x, 30), buf.c_str());
  }

  ImGui::End();
}

