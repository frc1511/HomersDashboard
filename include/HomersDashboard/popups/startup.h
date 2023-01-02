#pragma once

#include <ThunderDashboard/popups/popup.h>
#include <ThunderDashboard/nt_handler.h>

class StartupPopup: public frc1511::Popup {
public:
  static StartupPopup* get() {
    return &instance;
  }
  
  StartupPopup(StartupPopup const&) = delete;
  StartupPopup& operator=(StartupPopup const&) = delete;
  
  void present(bool* running) override;
  std::string get_name() override { return name; }

  struct Result {
    frc1511::NTHandler::Version version = frc1511::NTHandler::Version::V4;
    bool ds_running = true;
  };

  inline Result get_result() const { return result; }

private:
  StartupPopup();
  ~StartupPopup();

  std::string name = "Startup";

  Result result;

  static StartupPopup instance;
};