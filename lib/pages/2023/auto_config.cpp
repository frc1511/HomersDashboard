#include <HomersDashboard/pages/2023/auto_config.h>
#include <IconsFontAwesome5.h>

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

  static bool doing_auto = true;

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

  static int starting_location = 0;

  ImGui::PushID("Start Pos");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("Start Pos");
  ImGui::NextColumn();

  const char* starting_lcoation_names[] = {
    "Barrier Side",
    "Center",
    "Edge Side",
  };

  ImGui::Combo("##Start Pos", (int*)&starting_location, starting_lcoation_names, 3);

  ImGui::Columns(1);
  ImGui::PopID();

  ImGui::Separator();

  static int starting_gamepiece = -1;

  ImGui::PushID("Start GP");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("Start GP");
  ImGui::NextColumn();

  const char* starting_gp_names[] = {
    ICON_FA_CUBE " Cube",
    ICON_FA_ICE_CREAM " Cone",
    "None",
  };

  ImGui::Combo("##Start GP", (int*)&starting_gamepiece, starting_gp_names, 3);

  ImGui::Columns(1);
  ImGui::PopID();

  static int field_gamepiece = -1;

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

  ImGui::End();
}

AutoConfigPage AutoConfigPage::instance;
