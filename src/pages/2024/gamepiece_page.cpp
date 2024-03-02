#include <HomersDashboard/pages/2024/gamepiece_page.h>

using namespace y2024;

static const ImColor NO_GP_COLOR(43, 46, 54, 255);
static const ImColor GP_COLOR(255, 109, 0, 255);

void GamePiecePage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
  if (!ImGui::Begin(name(), running,
                    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoScrollWithMouse)) {
    ImGui::End();
    return;
  }

  const bool has_gamepiece = m_nt_handler.smart_dashboard()->GetBoolean(
      "thunderdashboard_2024_has_gamepiece", false);

  do {
    const ImVec2 avail(ImGui::GetContentRegionAvail());

    const float dim = avail.x < avail.y ? avail.x : avail.y;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    const ImGuiWindow* win = ImGui::GetCurrentWindow();
    if (win->SkipItems) break;

    const ImRect bb(win->DC.CursorPos, win->DC.CursorPos + ImVec2(dim, dim));
    ImGui::ItemSize(bb);

    const ImColor& color = has_gamepiece ? GP_COLOR : NO_GP_COLOR;

    // Draw note.
    draw_list->AddCircle(bb.GetCenter(), dim / 2.5f, color, 0, dim / 9.f);

  } while (false);

  ImGui::End();
}

