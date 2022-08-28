#include <pages/limelight.h>

LimelightPage::LimelightPage()
: CameraPage("intake_camera", "http://limelight-homer.local:5800") { }

LimelightPage::~LimelightPage() = default;

void LimelightPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("Limelight", running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }

  unsigned int tex(get_frame_texture());
  double ar(get_frame_aspect_ratio());

  ImVec2 avail(ImGui::GetContentRegionAvail());

  std::size_t cols(avail.x),
              rows(avail.x / ar);

  ImGui::Image(reinterpret_cast<void*>(tex), ImVec2(cols, rows));

  ImGui::End();
}

LimelightPage LimelightPage::instance;
