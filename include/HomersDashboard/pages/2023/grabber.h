#pragma once

#include <ThunderDashboard/pages/page.h>
#include <ThunderDashboard/nt_handler.h>

namespace y2023 {

class GrabberPage : public frc1511::Page {
public:
  static GrabberPage* get() {
    return &instance;
  }

  GrabberPage(GrabberPage const&) = delete;
  GrabberPage& operator=(GrabberPage const&) = delete;

  void init() override;
  void present(bool* running) override;

private:
  GrabberPage();
  ~GrabberPage();

  unsigned int cube_tex, cone_tex, cube_none_tex, cone_none_tex, gp_none_tex;

  static GrabberPage instance;
};

} // namespace y2023
