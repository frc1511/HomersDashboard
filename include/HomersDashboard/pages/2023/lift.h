#pragma once

#include <ThunderDashboard/pages/page.h>
#include <ThunderDashboard/nt_handler.h>

namespace y2023 {

class LiftPage : public frc1511::Page {
public:
  static LiftPage* get() {
    return &instance;
  }

  LiftPage(LiftPage const&) = delete;
  LiftPage& operator=(LiftPage const&) = delete;

  void init() override;
  void present(bool* running) override;

private:
  LiftPage();
  ~LiftPage();

  void show_lift();
  
  static LiftPage instance;
};

} // namespace y2023
