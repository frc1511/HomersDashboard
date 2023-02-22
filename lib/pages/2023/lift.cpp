#include <HomersDashboard/pages/2023/lift.h>
#include <imgui_internal.h>

#define COL_WIDTH 100

using namespace y2023;

LiftPage::LiftPage() = default;

LiftPage::~LiftPage() = default;

void LiftPage::init() { }

void LiftPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("2023 Lift", running, 
                    ImGuiWindowFlags_NoCollapse
                  | ImGuiWindowFlags_NoScrollbar
                  | ImGuiWindowFlags_NoScrollWithMouse)) {
    ImGui::End();
    return;
  }
  
  focused = ImGui::IsWindowFocused();

  show_lift();

  ImGui::End();
}

void LiftPage::show_lift() {
  ImDrawList* draw_list(ImGui::GetWindowDrawList());
  ImGuiWindow* win(ImGui::GetCurrentWindow());
  if (win->SkipItems) return;

  ImGui::SetScrollX(0);
  ImGui::SetScrollY(0);

  // --- Setup the canvas ---

  // Fit the canvas to the window.
  ImVec2 win_size(ImGui::GetContentRegionAvail());

  double aspect_ratio = 0.9 / 0.6;

  double dim_x(win_size.x),
         dim_y(win_size.y);

  if ((dim_x / dim_y) > aspect_ratio) {
    dim_x = dim_y * aspect_ratio;
  }
  else {
    dim_y = dim_x / aspect_ratio;
  }

  ImVec2 canvas(dim_x, dim_y);

  ImRect bb = ImRect(win->DC.CursorPos, win->DC.CursorPos + canvas);
  ImGui::ItemSize(bb);
  if (!ImGui::ItemAdd(bb, 0)) return;

  auto fix_pt = [&](ImVec2 pt) {
    pt.x /= 0.9;
    pt.y /= 0.6;
    pt = ImVec2(pt.x, 1 - pt.y) * (bb.Max - bb.Min) + bb.Min;
    return pt;
  };

  ImVec2 pivot_point(0.15 / 2, 0.35 - 0.15 / 2);

  double angle_percent            = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_lift_pivot_percent", 0.0);
  double extension_percent        = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_lift_extension_percent", 0.0);
  double target_angle_percent     = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_lift_pivot_target_percent", -1.0);
  double target_extension_percent = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_lift_extension_target_percent", -1.0);

  ImGuiStyle& style = ImGui::GetStyle();

  const double widths[] = { 0.05, 0.035, 0.02 };

  auto draw_arm = [draw_list, fix_pt, widths, pivot_point, style](double arm_angle, std::array<double, 3> arm_lengths, ImColor color, double line_width = 5.0f) {
    const double cos_angle_top(std::cos(arm_angle + M_PI_2));
    const double sin_angle_top(std::sin(arm_angle + M_PI_2));
    const double cos_angle_bottom(std::cos(arm_angle - M_PI_2));
    const double sin_angle_bottom(std::sin(arm_angle - M_PI_2));
    const double cos_angle(std::cos(arm_angle));
    const double sin_angle(std::sin(arm_angle));

    // Draw the arm.
    for (int i = 2; i >= 0; i--) {
      ImVec2 start_top(cos_angle_top * widths[i] + pivot_point.x, sin_angle_top * widths[i] + pivot_point.y);
      ImVec2 start_bottom(cos_angle_bottom * widths[i] + pivot_point.x, sin_angle_bottom * widths[i] + pivot_point.y);
      ImVec2 end_top(cos_angle * arm_lengths[i] + start_top.x, sin_angle * arm_lengths[i] + start_top.y);
      ImVec2 end_bottom(cos_angle * arm_lengths[i] + start_bottom.x, sin_angle * arm_lengths[i] + start_bottom.y);
      draw_list->AddQuadFilled(fix_pt(start_top), fix_pt(start_bottom), fix_pt(end_bottom), fix_pt(end_top), ImColor(style.Colors[ImGuiCol_WindowBg]));
      draw_list->AddQuad(fix_pt(start_top), fix_pt(start_bottom), fix_pt(end_bottom), fix_pt(end_top), color, line_width);
    }
  };

  ImColor color(255, 255, 255, 255);
  if (target_extension_percent != -1.0 && target_angle_percent != -1.0) {
    // -20 to 20 degrees, converted to radians.
    double target_angle = (target_angle_percent * 40 - 20) * M_PI / 180.0;

    draw_arm(target_angle, { 0.4, 0.4 + 0.05 + 0.2 * target_extension_percent, 0.4 + 0.05 + 0.35 * target_extension_percent }, style.Colors[ImGuiCol_Border], 5.0f);

    if (std::abs(target_angle_percent - angle_percent) < 0.01 && std::abs(target_extension_percent - extension_percent) < 0.01) {
      color = ImColor(0, 255, 0, 255);
    }
  }

  // -20 to 20 degrees, converted to radians.
  double angle = (angle_percent * 40 - 20) * M_PI / 180.0;

  draw_arm(angle, { 0.4, 0.4 + 0.05 + 0.2 * extension_percent, 0.4 + 0.05 + 0.35 * extension_percent }, color, 5.0f);

  // Draw the base.
  draw_list->AddRectFilled(fix_pt(ImVec2(0, 0)), fix_pt(ImVec2(0.15, 0.35)), ImColor(style.Colors[ImGuiCol_WindowBg]));
  draw_list->AddRect(fix_pt(ImVec2(0, 0)), fix_pt(ImVec2(0.15, 0.35)), ImColor(255, 255, 255, 255), 0.0f, 0, 5.0f);

  // Draw the pivot point.
  draw_list->AddCircle(fix_pt(pivot_point), 5.0f, ImColor(255, 255, 255, 255));
}

LiftPage LiftPage::instance;
