#include <HomersDashboard/pages/2024/hang_page.h>

using namespace y2024;

void HangPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(200, 600), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin(name(), running,
                    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoScrollWithMouse)) {
    ImGui::End();
    return;
  }

  float left_percent = m_nt_handler.smart_dashboard()->GetNumber(
      "thunderdashboard_2024_hang_left_percent", 0.0);

  float right_percent = m_nt_handler.smart_dashboard()->GetNumber(
      "thunderdashboard_2024_hang_right_percent", 0.0);

  left_percent = std::clamp(left_percent, 0.0f, 1.0f);
  right_percent = std::clamp(right_percent, 0.0f, 1.0f);

  const ImVec2 avail(ImGui::GetContentRegionAvail());

  draw_hang_side("Left", left_percent, avail);
  ImGui::SameLine();
  draw_hang_side("Right", right_percent, avail);

  ImGui::End();
}

void HangPage::draw_hang_side(const char* id, float percent, const ImVec2& avail) {
  ImGui::PushID(id);

  const ImGuiStyle& style = ImGui::GetStyle();

  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  const ImVec2 cursor_pos = ImGui::GetCursorScreenPos();

  const std::string text = fmt::format("{}%", int(percent * 100));
  const ImVec2 text_size = ImGui::CalcTextSize(text.c_str());

  const ImVec2 rect_size(avail.x / 2, avail.y - text_size.y);

  const ImVec2 rect_start =
      cursor_pos + ImVec2 {max(0.0f, ((text_size.x - rect_size.x) / 2.f)), 0};

  draw_list->AddRectFilled(rect_start, rect_start + rect_size,
                           ImGui::GetColorU32(ImGuiCol_FrameBg),
                           style.FrameRounding);
  draw_list->AddRectFilled(rect_start + ImVec2 {0, (1 - percent) * rect_size.y},
                           rect_start + rect_size * ImVec2 {1, 1},
                           ImGui::GetColorU32(ImGuiCol_PlotHistogram),
                           style.FrameRounding, ImDrawFlags_RoundCornersBottom);
  draw_list->AddText(rect_start + ImVec2 {(rect_size.x - text_size.x) / 2,
                                          style.FramePadding.y + (rect_size.y)},
                     ImGui::GetColorU32(ImGuiCol_Text), text.c_str());

  ImGui::Dummy(rect_size);
  ImGui::PopID();
}

