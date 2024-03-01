#pragma once

#include <HomersDashboard/pages/camera_page.h>

class IntakeCameraPage : public CameraPage {
public:
  IntakeCameraPage();
  ~IntakeCameraPage() = default;

  void present(bool* running) override;
  const char* name() const override { return "Intake Camera"; }
};

