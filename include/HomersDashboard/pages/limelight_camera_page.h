#pragma once

#include <HomersDashboard/pages/camera_page.h>

class LimelightCameraPage : public CameraPage {
public:
  LimelightCameraPage();
  ~LimelightCameraPage() = default;

  void present(bool* running) override;
  const char* name() const override { return "Limelight Camera"; }
};

