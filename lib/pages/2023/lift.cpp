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

  float dim_x(win_size.x),
        dim_y(win_size.y);

  const double aspect_ratio = 5.0 / 3.0;

  // Fit within the window size while maintaining aspect ratio.
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
    return ImVec2(pt.x, 1 - pt.y) * (bb.Max - bb.Min) + bb.Min;
  };

  draw_list->AddRect(fix_pt(ImVec2(0, 0)), fix_pt(ImVec2(0.15, 0.35)), ImColor(255, 255, 255, 255), 0.0f, 0, 5.0f);

  draw_list->AddCircle(fix_pt(ImVec2(0.15 / 2, 0.35 - 0.15 / 2)), 5.0f, ImColor(255, 255, 255, 255));
}

LiftPage LiftPage::instance;
