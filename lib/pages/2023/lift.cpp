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

  float dim = std::min(win_size.x, win_size.y);

  ImVec2 canvas(dim, dim);

  ImRect bb = ImRect(win->DC.CursorPos, win->DC.CursorPos + canvas);
  ImGui::ItemSize(bb);
  if (!ImGui::ItemAdd(bb, 0)) return;

  auto fix_pt = [&](ImVec2 pt) {
    pt = ImVec2(pt.x, 1 - pt.y) * (bb.Max - bb.Min) + bb.Min;
    return pt;
  };

  ImVec2 pivot_point(0.15 / 2, 0.35 - 0.15 / 2);

  static double angle_percent = 1.0;
  static double extension_percent = 1.0;

  static bool pivot_up = false;
  static bool extension_up = false;

  if (pivot_up) {
    angle_percent += 0.01;
    if (angle_percent > 1.0) {
      angle_percent = 1.0;
      pivot_up = false;
    }
  } else {
    angle_percent -= 0.01;
    if (angle_percent < 0.0) {
      angle_percent = 0.0;
      pivot_up = true;
    }
  }

  if (extension_up) {
    extension_percent += 0.01;
    if (extension_percent > 1.0) {
      extension_percent = 1.0;
      extension_up = false;
    }
  } else {
    extension_percent -= 0.01;
    if (extension_percent < 0.0) {
      extension_percent = 0.0;
      extension_up = true;
    }
  }

  // angle range from -20 to 20 degrees
  double angle = (angle_percent * 40 - 20) * M_PI / 180.0;

  const double stage_1_width = 0.05;
  const double stage_2_width = 0.035;
  const double stage_3_width = 0.02;
  const double stage_1_length = 0.4;

  const double stage_2_length = stage_1_length + 0.05 + 0.2 * extension_percent;
  const double stage_3_length = stage_1_length + 0.05 + 0.35 * extension_percent;

  ImGuiStyle& style = ImGui::GetStyle();

  // Stage 3
  ImVec2 start_top(std::cos(angle + M_PI_2) * stage_3_width + pivot_point.x, std::sin(angle + M_PI_2) * stage_3_width + pivot_point.y);
  ImVec2 start_bottom(std::cos(angle - M_PI_2) * stage_3_width + pivot_point.x, std::sin(angle - M_PI_2) * stage_3_width + pivot_point.y);
  ImVec2 end_top(std::cos(angle) * stage_3_length + start_top.x, std::sin(angle) * stage_3_length + start_top.y);
  ImVec2 end_bottom(std::cos(angle) * stage_3_length + start_bottom.x, std::sin(angle) * stage_3_length + start_bottom.y);
  draw_list->AddQuadFilled(fix_pt(start_top), fix_pt(start_bottom), fix_pt(end_bottom), fix_pt(end_top), ImColor(style.Colors[ImGuiCol_WindowBg]));
  draw_list->AddQuad(fix_pt(start_top), fix_pt(start_bottom), fix_pt(end_bottom), fix_pt(end_top), ImColor(255, 255, 255, 255), 5.0f);

  // Stage 2
  start_top = ImVec2(std::cos(angle + M_PI_2) * stage_2_width + pivot_point.x, std::sin(angle + M_PI_2) * stage_2_width + pivot_point.y);
  start_bottom = ImVec2(std::cos(angle - M_PI_2) * stage_2_width + pivot_point.x, std::sin(angle - M_PI_2) * stage_2_width + pivot_point.y);
  end_top = ImVec2(std::cos(angle) * stage_2_length + start_top.x, std::sin(angle) * stage_2_length + start_top.y);
  end_bottom = ImVec2(std::cos(angle) * stage_2_length + start_bottom.x, std::sin(angle) * stage_2_length + start_bottom.y);
  draw_list->AddQuadFilled(fix_pt(start_top), fix_pt(start_bottom), fix_pt(end_bottom), fix_pt(end_top), ImColor(style.Colors[ImGuiCol_WindowBg]));
  draw_list->AddQuad(fix_pt(start_top), fix_pt(start_bottom), fix_pt(end_bottom), fix_pt(end_top), ImColor(255, 255, 255, 255), 5.0f);

  // Stage 1
  start_top = ImVec2(std::cos(angle + M_PI_2) * stage_1_width + pivot_point.x, std::sin(angle + M_PI_2) * stage_1_width + pivot_point.y);
  start_bottom = ImVec2(std::cos(angle - M_PI_2) * stage_1_width + pivot_point.x, std::sin(angle - M_PI_2) * stage_1_width + pivot_point.y);
  end_top = ImVec2(std::cos(angle) * stage_1_length + start_top.x, std::sin(angle) * stage_1_length + start_top.y);
  end_bottom = ImVec2(std::cos(angle) * stage_1_length + start_bottom.x, std::sin(angle) * stage_1_length + start_bottom.y);
  draw_list->AddQuadFilled(fix_pt(start_top), fix_pt(start_bottom), fix_pt(end_bottom), fix_pt(end_top), ImColor(style.Colors[ImGuiCol_WindowBg]));
  draw_list->AddQuad(fix_pt(start_top), fix_pt(start_bottom), fix_pt(end_bottom), fix_pt(end_top), ImColor(255, 255, 255, 255), 5.0f);

  draw_list->AddRectFilled(fix_pt(ImVec2(0, 0)), fix_pt(ImVec2(0.15, 0.35)), ImColor(style.Colors[ImGuiCol_WindowBg]));
  draw_list->AddRect(fix_pt(ImVec2(0, 0)), fix_pt(ImVec2(0.15, 0.35)), ImColor(255, 255, 255, 255), 0.0f, 0, 5.0f);

  draw_list->AddCircle(fix_pt(pivot_point), 5.0f, ImColor(255, 255, 255, 255));
}

LiftPage LiftPage::instance;
