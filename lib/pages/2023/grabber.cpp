#include <HomersDashboard/pages/2023/grabber.h>
#include <imgui_internal.h>

#define COL_WIDTH 100

using namespace y2023;

GrabberPage::GrabberPage() = default;

GrabberPage::~GrabberPage() = default;

void GrabberPage::init() { }

void GrabberPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("2023 Grabber", running, 
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

GrabberPage GrabberPage::instance;
