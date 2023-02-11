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

  // Stuff here...

  ImGui::End();
}

LiftPage LiftPage::instance;
