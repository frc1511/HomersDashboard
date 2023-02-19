#include <HomersDashboard/pages/robot_position.h>
#include <HomersDashboard/utils.h>

#include <_2022_field_png.h>

RobotPositionPage::RobotPositionPage() = default;

RobotPositionPage::~RobotPositionPage() = default;

void RobotPositionPage::init() {
  int width, height, nr_channels;
  int field_tex = Utils::generate_texture_from_memory(_2022_field_png, _2022_field_png_size, &width, &height, &nr_channels);

  field_ar = static_cast<double>(width) / static_cast<double>(height);
}

void RobotPositionPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("Robot Position", running,
                    ImGuiWindowFlags_NoCollapse
                  | ImGuiWindowFlags_NoScrollbar
                  | ImGuiWindowFlags_NoScrollWithMouse)) {
    ImGui::End();
    return;
  }
  
  focused = ImGui::IsWindowFocused();

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
  ImVec2 win_size(ImGui::GetContentRegionAvail());

  float dim_x(win_size.x),
        dim_y(win_size.y);

  // Fit within the window size while maintaining aspect ratio.
  if ((dim_x / dim_y) > field_ar) {
    dim_x = dim_y * field_ar;
  }
  else {
    dim_y = dim_x / field_ar;
  }

  ImVec2 canvas(dim_x, dim_y);

  bb = ImRect(win->DC.CursorPos, win->DC.CursorPos + canvas);

  ImGui::ItemSize(bb);
  if (!ImGui::ItemAdd(bb, 0)) return;

  draw_list->AddImage(reinterpret_cast<void*>(field_tex), bb.Min, bb.Max);

  ImVec2 pt(frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_drive_x_pos", 0.0), frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_drive_y_pos", 0.0));
  double ang(frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_drive_ang", 0.0));

  ImVec2 pt0(to_draw_coord(pt)),
         pt1(to_draw_coord(ImVec2((std::cos(ang) * 1.0f) + pt.x, (std::sin(ang) * 1.0f) + pt.y)));

  draw_list->AddCircle(pt0, 5.0f, ImColor(252, 186, 3, 255), 0, 2.0f);
  draw_list->AddLine(pt0, pt1, ImColor(252, 186, 3, 255), 2.0f);
}

ImVec2 RobotPositionPage::to_draw_coord(ImVec2 pt) const {
  pt = adjust_field_coord(pt);

  pt.x /= FIELD_X;
  pt.y /= FIELD_Y;
  pt = ImVec2(pt.x, 1 - pt.y) * (bb.Max - bb.Min) + bb.Min;

  return pt;
}

ImVec2 RobotPositionPage::adjust_field_coord(ImVec2 pt) const {
  pt.x /= FIELD_X;
  pt.y /= FIELD_Y;

  pt.x *= (max.x - min.x);
  pt.y *= (max.y - min.y);

  pt.x += min.x;
  pt.y += min.y;

  pt.x *= FIELD_X;
  pt.y *= FIELD_Y;

  return pt;
}

RobotPositionPage RobotPositionPage::instance;
