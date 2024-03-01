#include <HomersDashboard/pages/limelight_camera_page.h>

LimelightCameraPage::LimelightCameraPage()
  : CameraPage("limelight", "http://limelight-homer.local:5800/stream.mjpg") {}

void LimelightCameraPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin(name(), running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }

  const unsigned int tex(get_frame_texture());
  const double ar(get_frame_aspect_ratio());

  ImVec2 avail(ImGui::GetContentRegionAvail());

  std::size_t cols(avail.x), rows(avail.x / ar);

  ImGui::Image(reinterpret_cast<void*>(tex), ImVec2(cols, rows));

  ImGui::End();
}

