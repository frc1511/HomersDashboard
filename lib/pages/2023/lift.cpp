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

  draw_list->AddRect(fix_pt(ImVec2(0, 0)), fix_pt(ImVec2(0.15, 0.35)), ImColor(255, 255, 255, 255), 0.0f, 0, 5.0f);

  ImVec2 pivot_point(0.15 / 2, 0.35 - 0.15 / 2);

  draw_list->AddCircle(fix_pt(pivot_point), 5.0f, ImColor(255, 255, 255, 255));

  double angle = M_PI_4;

  // Stage 1

  ImVec2 start_top(std::cos(angle + M_PI_2) * 0.1 + pivot_point.x, std::sin(angle + M_PI_2) * 0.1 + pivot_point.y);
  ImVec2 start_bottom(std::cos(angle - M_PI_2) * 0.1 + pivot_point.x, std::sin(angle - M_PI_2) * 0.1 + pivot_point.y);
  draw_list->AddLine(fix_pt(pivot_point), fix_pt(start_top), ImColor(0, 255, 0, 128), 5.0f);
  draw_list->AddLine(fix_pt(pivot_point), fix_pt(start_bottom), ImColor(255, 0, 0, 128), 5.0f);

  ImVec2 end_top(std::cos(angle) * 0.4 + start_top.x, std::sin(angle) * 0.4 + start_top.y);
  ImVec2 end_bottom(std::cos(angle) * 0.4 + start_bottom.x, std::sin(angle) * 0.4 + start_bottom.y);
  draw_list->AddLine(fix_pt(start_top), fix_pt(end_top), ImColor(0, 255, 0, 128), 5.0f);
  draw_list->AddLine(fix_pt(start_bottom), fix_pt(end_bottom), ImColor(255, 0, 0, 128), 5.0f);

  // Stage 2
  start_top = ImVec2(std::cos(angle + M_PI_2) * 0.06 + pivot_point.x, std::sin(angle + M_PI_2) * 0.06 + pivot_point.y);
  start_bottom = ImVec2(std::cos(angle - M_PI_2) * 0.06 + pivot_point.x, std::sin(angle - M_PI_2) * 0.06 + pivot_point.y);
  draw_list->AddLine(fix_pt(pivot_point), fix_pt(start_top), ImColor(0, 255, 0, 128), 5.0f);
  draw_list->AddLine(fix_pt(pivot_point), fix_pt(start_bottom), ImColor(255, 0, 0, 128), 5.0f);

  end_top = ImVec2(std::cos(angle) * 0.7 + start_top.x, std::sin(angle) * 0.7 + start_top.y);
  end_bottom = ImVec2(std::cos(angle) * 0.7 + start_bottom.x, std::sin(angle) * 0.7 + start_bottom.y);
  draw_list->AddLine(fix_pt(start_top), fix_pt(end_top), ImColor(0, 255, 0, 128), 5.0f);
  draw_list->AddLine(fix_pt(start_bottom), fix_pt(end_bottom), ImColor(255, 0, 0, 128), 5.0f);

  // Stage 3
  start_top = ImVec2(std::cos(angle + M_PI_2) * 0.03 + pivot_point.x, std::sin(angle + M_PI_2) * 0.03 + pivot_point.y);
  start_bottom = ImVec2(std::cos(angle - M_PI_2) * 0.03 + pivot_point.x, std::sin(angle - M_PI_2) * 0.03 + pivot_point.y);
  draw_list->AddLine(fix_pt(pivot_point), fix_pt(start_top), ImColor(0, 255, 0, 128), 5.0f);
  draw_list->AddLine(fix_pt(pivot_point), fix_pt(start_bottom), ImColor(255, 0, 0, 128), 5.0f);

  end_top = ImVec2(std::cos(angle) * 0.9 + start_top.x, std::sin(angle) * 0.9 + start_top.y);
  end_bottom = ImVec2(std::cos(angle) * 0.9 + start_bottom.x, std::sin(angle) * 0.9 + start_bottom.y);
  draw_list->AddLine(fix_pt(start_top), fix_pt(end_top), ImColor(0, 255, 0, 128), 5.0f);
  draw_list->AddLine(fix_pt(start_bottom), fix_pt(end_bottom), ImColor(255, 0, 0, 128), 5.0f);
}

LiftPage LiftPage::instance;
