#include <HomersDashboard/pages/intake_camera_page.h>

void IntakeCameraPage::init(bool host_local) {
  const char* url = "http://roborio-1511-frc.local:1181/stream.mjpg";
  const char* name = "intake_camera";

  if (host_local) {
    url = "http://127.0.0.1:1181/stream.mjpg";
    m_local_camera = frc::CameraServer::StartAutomaticCapture(0);
    m_local_stream = frc::CameraServer::PutVideo(name, 320, 240);
  }

  CameraPage::init(name, url);
}

void IntakeCameraPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin(name(), running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }

  const double ar = get_frame_aspect_ratio();

  const ImVec2 avail(ImGui::GetContentRegionAvail());

  std::size_t x = avail.x, y = avail.x / ar;

  ImGui::Image(get_frame_texture().id(), ImVec2(x, y));

  ImGui::End();
}

