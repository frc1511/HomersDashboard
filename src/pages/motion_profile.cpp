#include <pages/motion_profile.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <networktables/TableEntryListener.h>

MotionProfilePage::MotionProfilePage() { }

MotionProfilePage::~MotionProfilePage() { }

void MotionProfilePage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
  
  if (!ImGui::Begin("Motion Profile", running,
                ImGuiWindowFlags_NoCollapse
              | ImGuiWindowFlags_NoScrollbar
              | ImGuiWindowFlags_NoScrollWithMouse
              | ImGuiWindowFlags_UnsavedDocument * unsaved
              )) {

    ImGui::End();
    return;
  }

  focused = ImGui::IsWindowFocused();
  
  ImGui::End();
}

MotionProfilePage MotionProfilePage::instance {};