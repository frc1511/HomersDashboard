#include <HomersDashboard/pages/2023/lift_page.h>

static const double STAGE_WIDTHS[] = {0.05, 0.035, 0.02};
static const ImVec2 PIVOT_POINT(0.15 / 2, 0.35 - 0.15 / 2);

static const ImColor BASE_COLOR(255, 255, 255, 255);
static const ImColor AT_TARGET_COLOR(0, 255, 0, 255);
static const ImColor TARGET_COLOR(43, 46, 54, 255);

using namespace y2023;

void LiftPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin(name(), running,
                    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoScrollWithMouse)) {
    ImGui::End();
    return;
  }

  draw_lift();

  ImGui::End();
}

ImVec2 LiftPage::fix_pt(ImVec2 pt) const {
  pt.x /= 0.9;
  pt.y /= 0.6;
  pt = ImVec2(pt.x, 1 - pt.y) * (m_bb.Max - m_bb.Min) + m_bb.Min;
  return pt;
}

std::array<double, 3> LiftPage::get_stage_lengths(double extension_percent) {
  return {
      0.4,                            // Stage 1
      0.45 + 0.2 * extension_percent, // Stage 2
      0.45 + 0.35 * extension_percent // Stage 3
  };
}

void LiftPage::draw_lift() {
  m_draw_list = ImGui::GetWindowDrawList();
  ImGuiWindow* win(ImGui::GetCurrentWindow());
  if (win->SkipItems) return;

  ImGui::SetScrollX(0);
  ImGui::SetScrollY(0);

  //
  // Setup the canvas.
  //

  // Fit the canvas to the window.
  const ImVec2 win_size(ImGui::GetContentRegionAvail());
  double aspect_ratio = 0.9 / 0.6;

  double dim_x(win_size.x), dim_y(win_size.y);

  if ((dim_x / dim_y) > aspect_ratio) {
    dim_x = dim_y * aspect_ratio;
  } else {
    dim_y = dim_x / aspect_ratio;
  }

  const ImVec2 canvas(dim_x, dim_y);

  m_bb = ImRect(win->DC.CursorPos, win->DC.CursorPos + canvas);
  ImGui::ItemSize(m_bb);
  if (!ImGui::ItemAdd(m_bb, 0)) return;

  //
  // Get angle/extension.
  //

  const double angle_percent = m_nt_handler.smart_dashboard()->GetNumber(
      "thunderdashboard_lift_pivot_percent", 0.0);

  const double extension_percent = m_nt_handler.smart_dashboard()->GetNumber(
      "thunderdashboard_lift_extension_percent", 0.0);

  const double target_angle_percent = m_nt_handler.smart_dashboard()->GetNumber(
      "thunderdashboard_lift_pivot_target_percent", -1.0);

  const double target_extension_percent =
      m_nt_handler.smart_dashboard()->GetNumber(
          "thunderdashboard_lift_extension_target_percent", -1.0);

  //
  // Draw arm.
  //

  ImGuiStyle& style = ImGui::GetStyle();

  ImColor arm_color = BASE_COLOR;
  if (target_extension_percent != -1.0 && target_angle_percent != -1.0) {
    // -20 to 20 degrees, converted to radians.
    const double target_angle =
        (target_angle_percent * 40 - 20) * (std::numbers::pi / 180.0);

    draw_arm(target_angle, get_stage_lengths(target_extension_percent),
             TARGET_COLOR, 5.0f);

    if (std::abs(target_angle_percent - angle_percent) < 0.01 &&
        std::abs(target_extension_percent - extension_percent) < 0.01) {
      arm_color = AT_TARGET_COLOR;
    }
  }

  // -20 to 20 degrees, converted to radians.
  const double angle = (angle_percent * 40 - 20) * (std::numbers::pi / 180.0);

  draw_arm(angle, get_stage_lengths(extension_percent), arm_color, 5.0f);

  // Draw the base.
  m_draw_list->AddRectFilled(fix_pt(ImVec2(0, 0)), fix_pt(ImVec2(0.15, 0.35)),
                             ImColor(style.Colors[ImGuiCol_WindowBg]));
  m_draw_list->AddRect(fix_pt(ImVec2(0, 0)), fix_pt(ImVec2(0.15, 0.35)),
                       BASE_COLOR, 0.0f, 0, 5.0f);

  // Draw the pivot point.
  m_draw_list->AddCircle(fix_pt(PIVOT_POINT), 5.0f, BASE_COLOR);
}

void LiftPage::draw_arm(double angle, std::array<double, 3> stage_lengths,
                        ImColor color, double width) {

  const double cos_angle_top(std::cos(angle + 0.5 * std::numbers::pi));
  const double sin_angle_top(std::sin(angle + 0.5 * std::numbers::pi));
  const double cos_angle_bottom(std::cos(angle - 0.5 * std::numbers::pi));
  const double sin_angle_bottom(std::sin(angle - 0.5 * std::numbers::pi));
  const double cos_angle(std::cos(angle));
  const double sin_angle(std::sin(angle));

  const ImGuiStyle& style = ImGui::GetStyle();

  // Draw the arm stages.
  for (int i = 2; i >= 0; i--) {
    ImVec2 start_top(cos_angle_top * STAGE_WIDTHS[i] + PIVOT_POINT.x,
                     sin_angle_top * STAGE_WIDTHS[i] + PIVOT_POINT.y);
    ImVec2 start_bottom(cos_angle_bottom * STAGE_WIDTHS[i] + PIVOT_POINT.x,
                        sin_angle_bottom * STAGE_WIDTHS[i] + PIVOT_POINT.y);
    ImVec2 end_top(cos_angle * stage_lengths[i] + start_top.x,
                   sin_angle * stage_lengths[i] + start_top.y);
    ImVec2 end_bottom(cos_angle * stage_lengths[i] + start_bottom.x,
                      sin_angle * stage_lengths[i] + start_bottom.y);

    // Hide the stages retracted into the arm.
    m_draw_list->AddQuadFilled(fix_pt(start_top), fix_pt(start_bottom),
                               fix_pt(end_bottom), fix_pt(end_top),
                               ImColor(style.Colors[ImGuiCol_WindowBg]));

    m_draw_list->AddQuad(fix_pt(start_top), fix_pt(start_bottom),
                         fix_pt(end_bottom), fix_pt(end_top), color, width);
  }
}

