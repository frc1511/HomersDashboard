#pragma once

#include <HomersDashboard/pages/camera_page.h>

#ifdef HD_WITH_CS
#include <cameraserver/CameraServer.h>
#endif

class IntakeCameraPage : public CameraPage {
#ifdef HD_WITH_CS
  cs::UsbCamera m_local_camera;
  cs::CvSource m_local_stream;
#endif

public:
  IntakeCameraPage() = default;
  ~IntakeCameraPage() = default;

  void init(bool host_local);

  void present(bool* running) override;
  const char* name() const override { return "Intake Camera"; }
};
