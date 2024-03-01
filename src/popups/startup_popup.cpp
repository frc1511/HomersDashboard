#include <HomersDashboard/popups/startup_popup.h>

#include <HomersDashboard/imgui_util.h>

#define COLUMN_WIDTH 180.f

void StartupPopup::present(bool* running) {
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), false,
                          ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2(420, 155));
  if (!ImGui::BeginPopupModal(name(), nullptr, ImGuiWindowFlags_NoResize)) {
    return;
  }

  {
    ImGuiScopedField field("NT Version", COLUMN_WIDTH);

    const char* version_names[] = {"V4 (2023 and later)",
                                   "V3 (2022 and earlier)"};

    ImGui::Combo("##NT Version", (int*)&m_settings.nt_version, version_names,
                 2);
  }
  {
    ImGuiScopedField field("DS Running", "Driver Startion Running",
                           COLUMN_WIDTH);

    ImGui::Checkbox("##DS Running", &m_settings.ds_running);
  }
  if (ImGui::TreeNodeEx("Advanced")) {
    {
      ImGuiScopedField field("Local NT Server", COLUMN_WIDTH);
      bool hovered = ImGui::IsItemHovered();

      ImGui::Checkbox("##Local NT Server", &m_settings.local_nt_server);
      hovered |= ImGui::IsItemHovered();
      if (hovered) {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted("Look for NT server at 127.0.0.1");
        ImGui::EndTooltip();
      }
    }

    ImGui::TreePop();
  }

  if (ImGui::Button("Ok")) {
    *running = false;
  }

  ImGui::EndPopup();
}

