#pragma once

#include <HomersDashboard/pages/camera_page.h>
#include <cameraserver/CameraServer.h>

class IntakeCameraPage : public CameraPage {
  cs::UsbCamera m_local_camera;
  cs::CvSource m_local_stream;

public:
  IntakeCameraPage() = default;
  ~IntakeCameraPage() = default;

  void init(bool host_local);

  void present(bool* running) override;
  const char* name() const override { return "Intake Camera"; }
};

