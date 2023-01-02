#include <HomersDashboard/popups/startup.h>

#define COL_WIDTH 180.0f

StartupPopup::StartupPopup() = default;

StartupPopup::~StartupPopup() = default;

void StartupPopup::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(420, 120), ImGuiCond_FirstUseEver);
  if (!ImGui::BeginPopupModal(name.c_str(), nullptr, ImGuiWindowFlags_None)) {
    return;
  }

  ImGui::PushID("NT Version");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("NT Version");
  ImGui::NextColumn();

  const char* version_names[] = {
    "V4 (2023 and later)",
    "V3 (2022 and earlier)"
  };

  ImGui::Combo("##NT Version", (int*)&result.version, version_names, 2);
  
  ImGui::Columns(1);
  ImGui::PopID();

  ImGui::PushID("DS Running");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("Is Driver Station Running?");
  ImGui::NextColumn();

  ImGui::Checkbox("##DS Running", &result.ds_running);

  ImGui::Columns(1);
  ImGui::PopID();

  ImGui::Spacing();

  if (ImGui::Button("Ok")) {
    goto close;
  }
  
  ImGui::EndPopup();
  return;

close:
  ImGui::CloseCurrentPopup();
  *running = false;
  ImGui::EndPopup();
}

StartupPopup StartupPopup::instance {};
