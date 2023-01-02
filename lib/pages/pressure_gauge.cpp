#include <HomersDashboard/pages/pressure_gauge.h>
#include <imgui_internal.h>
#include <ThunderDashboard/font_manager.h>
#include <ThunderDashboard/nt_handler.h>

#define COL_WIDTH 100

PressureGaugePage::PressureGaugePage() = default;

PressureGaugePage::~PressureGaugePage() = default;

void PressureGaugePage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(250, 250), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("Pressure Gauge", running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }
  
  focused = ImGui::IsWindowFocused();

  show_pressure_gauge();

  ImGui::End();
}

void PressureGaugePage::show_pressure_gauge() {
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  ImGuiWindow* win = ImGui::GetCurrentWindow();
  if (win->SkipItems) return;

  ImVec2 canvas = ImGui::GetContentRegionAvail();
  canvas.y *= 0.9f;

  if (canvas.x > canvas.y) {
    canvas.x = canvas.y;
  } else {
    canvas.y = canvas.x;
  }

  ImRect bb(win->DC.CursorPos, win->DC.CursorPos + canvas);
  ImGui::ItemSize(bb);
  if (!ImGui::ItemAdd(bb, 0)) return;

  double radius = canvas.x / 2.5f;

  draw_list->AddCircle(bb.GetCenter(), radius, ImColor(255, 255, 255, 255), 0, 3.0f);

  draw_list->AddCircleFilled(bb.GetCenter(), 3.0f, ImColor(252, 186, 3, 255), 0);

  double low = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("airpressure_low", 0.0);
  double high = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("airpressure_high", 130.0);
  double current = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("airpressure", 0.0);
  double iters = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("airpressure_iters", 20.0);

  auto get_pt = [&](double angle, double extra_dist = 0) -> ImVec2 {
    return ImVec2(
      bb.GetCenter().x + cos((angle - 90) * M_PI / 180) * (radius + extra_dist),
      bb.GetCenter().y + sin((angle - 90) * M_PI / 180) * (radius + extra_dist)
    );
  };

  double low_angle = -145, high_angle = 145;
  double current_angle = low_angle + ((current - low) / (high - low)) * (high_angle - low_angle);

  for (int i = 0; i < iters; i++) {
    double angle = low_angle + (i / (iters - 1.0)) * (high_angle - low_angle);
    draw_list->AddLine(get_pt(angle, -5), get_pt(angle, 5), ImColor(255, 255, 255, 255), 3.0f);

    double text_dist;
    if (std::abs(angle) > 90) {
      text_dist = angle > 0 ? 10 : 20;
    } else {
      text_dist = angle > 0 ? 18 : 25;
    }

    draw_list->AddText(get_pt(angle, text_dist), ImColor(255, 255, 255, 255), fmt::format("{:.0f}", low + (i / (iters - 1.0)) * (high - low)).c_str());
  }

  draw_list->AddLine(bb.GetCenter(), get_pt(current_angle, -10), ImColor(252, 186, 3, 255), 3.0f);

  ImGui::Text("Current Pressure: %.2f", current);
}

PressureGaugePage PressureGaugePage::instance;
