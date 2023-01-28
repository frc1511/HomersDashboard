#include <HomersDashboard/pages/2022/shooter_position.h>
#include <imgui_internal.h>

#define COL_WIDTH 50

using namespace y2022;

ShooterPositionPage::ShooterPositionPage() = default;

ShooterPositionPage::~ShooterPositionPage() = default;

void ShooterPositionPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("2022 Shooter Position", running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }
  
  focused = ImGui::IsWindowFocused();
  std::string buf;

  ImGui::PushID("RPM");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("RPM");
  ImGui::NextColumn();

  double rpm_max = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_rpm_max", 2600.0);
  double rpm = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_rpm", 0.0);

  double rpm_pct = rpm / rpm_max;

  buf = fmt::format("{}", (int)rpm);
  ImGui::ProgressBar(rpm_pct, ImVec2(ImGui::GetContentRegionAvail().x, 30), buf.c_str());

  ImGui::Columns(1);
  ImGui::PopID();

  ImGui::PushID("Hood");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("Hood");
  ImGui::NextColumn();

  double hood_min = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_hood_min", 0.0);
  double hood_max = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_hood_max", 1.0);
  double hood = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_hood", 0.0);

  double hood_pct = (hood - hood_min) / (hood_max - hood_min);

  buf = fmt::format("{:.3f}", hood - hood_min);
  ImGui::ProgressBar(hood_pct, ImVec2(ImGui::GetContentRegionAvail().x, 30), buf.c_str());

  ImGui::Columns(1);
  ImGui::PopID();

  ImGui::End();
}

ShooterPositionPage ShooterPositionPage::instance;
