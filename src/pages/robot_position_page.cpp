#include <HomersDashboard/pages/robot_position_page.h>

#include <_2024_field_png.h>

static const ImRect FIELD_RECT(ImVec2(0.080078125f, 0.07861328125f),
                               ImVec2(0.919921875f, 0.90625f));

static const ImVec2 FIELD_SIZE(16.54175f, 8.0137f); // meters.

RobotPositionPage::RobotPositionPage(NTHandler& nt_handler)
  : m_nt_handler(nt_handler),
    m_field_tex(_2024_field_png, _2024_field_png_size),
    m_field_ar(static_cast<double>(m_field_tex.width()) /
               m_field_tex.height()) {}

void RobotPositionPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin(name(), running,
                    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoScrollWithMouse)) {
    ImGui::End();
    return;
  }

  show_robot_position();

  ImGui::End();
}

void RobotPositionPage::show_robot_position() {
  ImDrawList* draw_list(ImGui::GetWindowDrawList());
  ImGuiWindow* win(ImGui::GetCurrentWindow());
  if (win->SkipItems) return;

  ImGui::SetScrollX(0);
  ImGui::SetScrollY(0);

  // --- Setup the canvas ---

  // Fit the canvas to the window.
  const ImVec2 win_size(ImGui::GetContentRegionAvail());

  float dim_x(win_size.x), dim_y(win_size.y);

  // Fit within the window size while maintaining aspect ratio.
  if ((dim_x / dim_y) > m_field_ar) {
    dim_x = dim_y * m_field_ar;
  } else {
    dim_y = dim_x / m_field_ar;
  }

  ImVec2 canvas(dim_x, dim_y);

  m_bb = ImRect(win->DC.CursorPos, win->DC.CursorPos + canvas);

  ImGui::ItemSize(m_bb);
  if (!ImGui::ItemAdd(m_bb, 0)) return;

  draw_list->AddImage(m_field_tex.id(), m_bb.Min, m_bb.Max);

  const ImVec2 pt(m_nt_handler.smart_dashboard()->GetNumber(
                      "thunderdashboard_drive_x_pos", 0.0),
                  m_nt_handler.smart_dashboard()->GetNumber(
                      "thunderdashboard_drive_y_pos", 0.0));
  const float ang(m_nt_handler.smart_dashboard()->GetNumber(
      "thunderdashboard_drive_ang", 0.0));

  ImVec2 pt0(to_draw_coord(pt)),
      pt1(to_draw_coord(ImVec2((std::cos(ang) * 1.0f) + pt.x,
                               (std::sin(ang) * 1.0f) + pt.y)));

  draw_list->AddCircle(pt0, 5.0f, ImColor(252, 186, 3, 255), 0, 2.0f);
  draw_list->AddLine(pt0, pt1, ImColor(252, 186, 3, 255), 2.0f);

  ImGui::Text("x: %f,   y: %f,   rot: %f", pt.x, pt.y, ang);
}

ImVec2 RobotPositionPage::to_draw_coord(ImVec2 pt) const {
  pt = adjust_field_coord(pt);

  pt.x /= FIELD_SIZE.x;
  pt.y /= FIELD_SIZE.y;
  pt = ImVec2(pt.x, 1.f - pt.y) * (m_bb.Max - m_bb.Min) + m_bb.Min;

  return pt;
}

ImVec2 RobotPositionPage::adjust_field_coord(ImVec2 pt) const {
  pt.x /= FIELD_SIZE.x;
  pt.y /= FIELD_SIZE.y;

  pt.x *= (FIELD_RECT.Max.x - FIELD_RECT.Min.x);
  pt.y *= (FIELD_RECT.Max.y - FIELD_RECT.Min.y);

  pt.x += FIELD_RECT.Min.x;
  pt.y += FIELD_RECT.Min.y;

  pt.x *= FIELD_SIZE.x;
  pt.y *= FIELD_SIZE.y;

  return pt;
}
