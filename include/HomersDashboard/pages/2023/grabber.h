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

  static GrabberPage instance;
};

} // namespace y2023
