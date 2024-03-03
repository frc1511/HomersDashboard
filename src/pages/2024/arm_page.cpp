#include <HomersDashboard/pages/2024/arm_page.h>

using namespace y2024;

static const ImVec2 PIVOT_POINT(0.12f, 0.2f);

static const float ARM_LENGTH = 0.67f;
static const float ARM_WIDTH = 0.06f;

static const ImColor BASE_COLOR(255, 255, 255, 255);
static const ImColor AT_TARGET_COLOR(0, 255, 0, 255);
static const ImColor TARGET_COLOR(43, 46, 54, 255);

static const ImColor BUMPER_RED_COLOR(237, 28, 38, 255);
static const ImColor BUMPER_BLUE_COLOR(20, 21, 186, 255);

static const ImVec2 WHEEL_1_POS(0.115f, 0.072f);
static const ImVec2 WHEEL_2_POS(0.708f, 0.072f);

static const float WHEEL_RADIUS = 0.04f;
static const float PIVOT_RADIUS = 0.065f;

static const ImRect BUMPERS_RECT(ImVec2(0.041f, 0.17f), ImVec2(0.78f, 0.057f));

static const ImVec2 ARM_PTS[] = {
    ImVec2(0.f, -ARM_WIDTH / 2),
    ImVec2(ARM_LENGTH, -ARM_WIDTH / 2),
    ImVec2(ARM_LENGTH, ARM_WIDTH / 2),
    ImVec2(0.f, ARM_WIDTH / 2),
};

static const ImVec2 SHOOTER_PTS[] = {
    ImVec2(38.4f / 1700, (2048.f - 1772.9f) / 1700),
    ImVec2(346.3f / 1700, (2048.f - 2357.f) / 1700),
    ImVec2(132.7f / 1700, (2048.f - 2438.f) / 1700),
    ImVec2(-14.1f / 1700, (2048.f - 2152.2f) / 1700),
    ImVec2(-300.6f / 1700, (2048.f - 2048.2f) / 1700),
    ImVec2(-320.3f / 1700, (2048.f - 1918.3f) / 1700),
};

void ArmPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin(name(), running,
                    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoScrollWithMouse)) {
    ImGui::End();
    return;
  }

  draw_arm();

  ImGui::End();
}

ImVec2 ArmPage::rotate_pt_origin(ImVec2 origin, ImVec2 pt, float angle_rad) {
  const float cos_angle(std::cos(angle_rad));
  const float sin_angle(std::sin(angle_rad));

  const ImVec2 diff(pt - origin);
  return ImVec2(origin.x + diff.x * cos_angle - diff.y * sin_angle,
                origin.y + diff.x * sin_angle + diff.y * cos_angle);
}

void ArmPage::rotate_pts_origin(ImVec2 origin, ImVec2* pts, size_t num_pts,
                                float angle_rad) {
  for (size_t i = 0; i < num_pts; ++i) {
    pts[i] = rotate_pt_origin(origin, pts[i], angle_rad);
  }
}

void ArmPage::draw_arm() {
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

  float dim_x(win_size.x), dim_y(win_size.y);

  if ((dim_x / dim_y) > m_aspect_ratio) {
    dim_x = dim_y * m_aspect_ratio;
  } else {
    dim_y = dim_x / m_aspect_ratio;
  }

  const ImVec2 canvas(dim_x, dim_y);

  m_bb = ImRect(win->DC.CursorPos, win->DC.CursorPos + canvas);
  /* ImGui::ItemSize(m_bb); */
  /* if (!ImGui::ItemAdd(m_bb, 0)) return; */

  //
  // Get angle.
  //
  float angle_deg(m_nt_handler.smart_dashboard()->GetNumber(
      "thunderdashboard_2024_arm_pivot_deg", 0.0));

  angle_deg = std::clamp(angle_deg, 0.f, 85.f);

  float target_angle_deg(m_nt_handler.smart_dashboard()->GetNumber(
      "thunderdashboard_2024_arm_pivot_target_deg", -1.0));

  const bool no_target = float_eq(target_angle_deg, -1.f);
  if (!no_target) {
    target_angle_deg = std::clamp(target_angle_deg, 0.f, 85.f);
  }

  const auto to_rad = [](float deg) {
    return (deg + 5.f) * 3.14159f / 180.f;
  };

  const float angle_rad = to_rad(angle_deg);
  const float target_angle_rad = to_rad(target_angle_deg);

  bool at_target = false;

  //
  // Draw target.
  //
  if (!no_target) {
    draw_arm_angle(target_angle_rad, TARGET_COLOR);

    at_target = std::fabs(angle_deg - target_angle_deg) < 5.f;
  }

  //
  // Draw current.
  //
  draw_arm_angle(angle_rad, at_target ? AT_TARGET_COLOR : BASE_COLOR);

  //
  // Draw pivot point.
  //
  m_draw_list->AddCircleFilled(fix_pt(PIVOT_POINT), scale_value(PIVOT_RADIUS),
                               BASE_COLOR);

  //
  // Draw wheels.
  //
  m_draw_list->AddCircleFilled(fix_pt(WHEEL_1_POS), scale_value(WHEEL_RADIUS),
                               BASE_COLOR);
  m_draw_list->AddCircleFilled(fix_pt(WHEEL_2_POS), scale_value(WHEEL_RADIUS),
                               BASE_COLOR);

  //
  // Draw bumpers.
  //
  const ImColor bumper_color =
      m_nt_handler.get_alliance() == NTHandler::Alliance::RED
          ? BUMPER_RED_COLOR
          : BUMPER_BLUE_COLOR;

  m_draw_list->AddRectFilled(fix_pt(BUMPERS_RECT.Min), fix_pt(BUMPERS_RECT.Max),
                             bumper_color, 5.f, 0);

  //
  // Draw text.
  //
  ImGui::SameLine(win_size.x - 80);
  ImGui::Text("Current: %.1f°", angle_deg);
  if (!no_target) {
    ImGui::Dummy(ImVec2(0, 0));
    ImGui::SameLine(win_size.x - 80);
    ImGui::Text("Target: %.1f°", target_angle_deg);
  }

}

void ArmPage::draw_polygon(const ImVec2* pts, size_t num_pts, ImVec2 offset,
                           ImVec2 origin, float angle_rad, ImColor color) {

  //
  // Rotate and translate the points.
  //
  ImVec2* new_pts = (ImVec2*)alloca(sizeof(ImVec2) * num_pts);

  for (size_t i = 0; i < num_pts; ++i) {
    new_pts[i] = fix_pt(pts[i] + offset);
  }
  rotate_pts_origin(fix_pt(origin), new_pts, num_pts, angle_rad);

  //
  // Draw the polygon.
  //
  m_draw_list->AddConvexPolyFilled(new_pts, num_pts, color);
}

void ArmPage::draw_arm_angle(float angle_rad, ImColor color) {

  //
  // Arm.
  //
  draw_polygon(ARM_PTS, sizeof(ARM_PTS) / sizeof(ImVec2), PIVOT_POINT,
               PIVOT_POINT, -angle_rad, color);

  //
  // Shooter/Intake.
  //
  draw_polygon(SHOOTER_PTS, sizeof(SHOOTER_PTS) / sizeof(ImVec2),
               ImVec2(PIVOT_POINT.x + ARM_LENGTH, PIVOT_POINT.y), PIVOT_POINT,
               -angle_rad, color);
}

