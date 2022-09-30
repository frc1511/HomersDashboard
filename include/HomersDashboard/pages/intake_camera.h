#pragma once

#include <ThunderDashboard/pages/camera_page.h>

class IntakeCameraPage : public frc1511::CameraPage {
public:
  static IntakeCameraPage* get() {
    return &instance;
  }

  IntakeCameraPage(IntakeCameraPage const&) = delete;
  IntakeCameraPage& operator=(IntakeCameraPage const&) = delete;

  void present(bool* running) override;

private:
  IntakeCameraPage();
  ~IntakeCameraPage();

  static IntakeCameraPage instance;
};
