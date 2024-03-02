#pragma once

#include <HomersDashboard/homers_dashboard.h>
#include <HomersDashboard/nt_handler.h>
#include <HomersDashboard/popups/popup.h>

struct StartupSettings {
  NTHandler::Version nt_version = NTHandler::Version::V4;
  bool ds_running = true;
  bool local_nt_server = false;
  bool host_camera_server = false;
};

class StartupPopup : public Popup {
public:
  const char* name() override { return "##Startup"; }
  void present(bool* running) override;

  StartupSettings settings() const { return m_settings; }

private:
  StartupSettings m_settings {};
};

