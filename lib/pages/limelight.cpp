#include <HomersDashboard/pages/limelight.h>

LimelightPage::LimelightPage()
: frc1511::CameraPage("limelight", "http://limelight-homer.local:5800/stream.mjpg") { }

LimelightPage::~LimelightPage() = default;

void LimelightPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("Limelight", running, ImGuiWindowFlags_NoCollapse)) {
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

LimelightPage LimelightPage::instance;
