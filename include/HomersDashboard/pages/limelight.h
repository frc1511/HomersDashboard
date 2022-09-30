#pragma once

#include <ThunderDashboard/pages/camera_page.h>

class LimelightPage : public frc1511::CameraPage {
public:
  static LimelightPage* get() {
    return &instance;
  }

  LimelightPage(LimelightPage const&) = delete;
  LimelightPage& operator=(LimelightPage const&) = delete;

  void present(bool* running) override;

private:
  LimelightPage();
  ~LimelightPage();

  static LimelightPage instance;
};
