#include <HomersDashboard/pages/2023/auto_config.h>
#include <IconsFontAwesome5.h>
#include <HomersDashboard/pages/auto_chooser.h>

#define COL_WIDTH 100

using namespace y2023;

AutoConfigPage::AutoConfigPage() = default;

AutoConfigPage::~AutoConfigPage() = default;

void AutoConfigPage::init() { }

void AutoConfigPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("2023 Auto Config", running, 
                    ImGuiWindowFlags_NoCollapse
                  | ImGuiWindowFlags_NoScrollbar
                  | ImGuiWindowFlags_NoScrollWithMouse)) {
    ImGui::End();
    return;
  }
  
  focused = ImGui::IsWindowFocused();

  // Doing Auto

  ImGui::PushID("Doing Auto");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("Doing Auto");
  ImGui::NextColumn();
  ImGui::Checkbox("##Doing Auto", &doing_auto);
  ImGui::Columns(1);
  ImGui::PopID();

  if (!doing_auto) {
    ImGui::End();
    return;
  }

  // Auto Delay

  ImGui::PushID("Auto Delay");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("Auto Delay");
  ImGui::NextColumn();

  float auto_delay = AutoChooserPage::get()->get_auto_delay();
  if (ImGui::DragFloat("##Auto Delay", &auto_delay, 0.1f, 0.0f, 0.0f, "%.2f s")) {
    AutoChooserPage::get()->set_auto_delay(auto_delay);
  }

  ImGui::Columns(1);
  ImGui::PopID();

  ImGui::Separator();

  // Start position

  ImGui::PushID("Start Pos");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("Start Zone");
  ImGui::NextColumn();

  const char* starting_location_names[] = {
    "Barrier Side",
    "Center",
    "Edge Side",
  };

  ImGui::Combo("##Start Pos", (int*)&starting_location, starting_location_names, 3);

  ImGui::Columns(1);
  ImGui::PopID();

  // Starting GamePiece

  const char* starting_gp_names[] = {
    ICON_FA_CUBE " Cube",
    ICON_FA_ICE_CREAM " Cone",
  };

  ImGui::PushID("Start GP");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("Start GP");
  ImGui::NextColumn();

  ImGui::Combo("##Start GP", (int*)&starting_gamepiece, starting_gp_names, 2);

  ImGui::Columns(1);
  ImGui::PopID();

  // Starting Action

  ImGui::PushID("Start Action");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("Start Action");
  ImGui::NextColumn();

  if (starting_location == 1) {
    const char* starting_action_names[] = {
      "Score preload, balance",
      "Score preload, traverse, collect GP 2",
    };

    ImGui::Combo("##Start Action", (int*)&starting_action, starting_action_names, 2);
  }
  else {
    ImGui::Text("Score preloaded GP");
  }

  ImGui::Columns(1);
  ImGui::PopID();

  if (starting_location == 1 && starting_action == 0) {
    ImGui::End();
    return;
  }

  // Field GamePiece to collect

  ImGui::PushID("Field GP");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("Field GP");
  ImGui::NextColumn();

  const char* field_gp_names[] = {
    ICON_FA_CUBE " Cube",
    ICON_FA_ICE_CREAM " Cone",
  };

  ImGui::Combo("##Field GP", (int*)&field_gamepiece, field_gp_names, 2);

  ImGui::Columns(1);
  ImGui::PopID();

  // Final Action

  const char* barrier_final_action_names[] = {
      "Do nothing",
      "Score GP 1",
      "Balance on CS"
  };
  const char* center_final_action_names[] = {
      "Do nothing",
      "Balance on CS"
  };
  const char* edge_final_action_names[] = {
      "Do nothing",
      "Score GP 4",
      "Balance on CS"
  };

  const char** final_action_names = nullptr;
  int final_action_name_num = 0;
  switch (starting_location) {
  case 0:
    final_action_names = barrier_final_action_names;
    final_action_name_num = 3;
    break;
  case 1:
    final_action_names = center_final_action_names;
    final_action_name_num = 2;
    break;
  case 2:
    final_action_names = edge_final_action_names;
    final_action_name_num = 3;
    break;
  default:
    break;
  }

  ImGui::PushID("Final Action");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("Final Action");
  ImGui::NextColumn();

  ImGui::Combo("##Final Action", (int*)&final_action, final_action_names, final_action_name_num);

  ImGui::Columns(1);
  ImGui::PopID();

  ImGui::End();
}

AutoConfigPage AutoConfigPage::instance;
