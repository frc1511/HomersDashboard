#include <HomersDashboard/pages/intake_camera_page.h>

IntakeCameraPage::IntakeCameraPage()
  : CameraPage("intake_camera",
               "http://roborio-1511-frc.local:1181/stream.mjpg") {}

void IntakeCameraPage::present(bool* running) {
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

