#include <HomersDashboard/pages/limelight_camera_page.h>

void LimelightCameraPage::init() {
  CameraPage::init("limelight",
                   "http://limelight-homer.local:5800/stream.mjpg");
}

void LimelightCameraPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin(name(), running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }

  const float ar = get_frame_aspect_ratio();

  const ImVec2 avail(ImGui::GetContentRegionAvail());

  const float x(avail.x), y(avail.x / ar);

  ImGui::Image(get_frame_texture().id(), ImVec2(x, y));

  ImGui::End();
}

