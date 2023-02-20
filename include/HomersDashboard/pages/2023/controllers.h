#pragma once

#include <ThunderDashboard/pages/page.h>
#include <ThunderDashboard/nt_handler.h>

namespace y2023 {

class ControllerPage : public frc1511::Page {
public:
  static ControllerPage* get() {
    return &instance;
  }

  ControllerPage(ControllerPage const&) = delete;
  ControllerPage& operator=(ControllerPage const&) = delete;

  void init() override;
  void present(bool* running) override;

private:
  ControllerPage();
  ~ControllerPage();

  unsigned int driver_tex, aux_tex, aux_manual_tex;
  double aspect_ratio;

  static ControllerPage instance;
};

} // namespace y2023
