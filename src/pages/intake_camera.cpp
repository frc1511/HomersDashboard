#include <pages/intake_camera.h>

IntakeCameraPage::IntakeCameraPage()
: CameraPage("intake_camera", "http://roborio-1511-frc.local:1181/stream.mjpg") { }

IntakeCameraPage::~IntakeCameraPage() = default;

void IntakeCameraPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("Intake Camera", running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }

  focused = ImGui::IsWindowFocused();

  unsigned int tex(get_frame_texture());
  double ar(get_frame_aspect_ratio());

  ImVec2 avail(ImGui::GetContentRegionAvail());

  std::size_t cols(avail.x),
              rows(avail.x / ar);

  ImGui::Image(reinterpret_cast<void*>(tex), ImVec2(cols, rows));

  ImGui::End();
}

IntakeCameraPage IntakeCameraPage::instance;
