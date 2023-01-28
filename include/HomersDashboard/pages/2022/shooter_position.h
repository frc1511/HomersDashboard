#pragma once

#include <ThunderDashboard/pages/page.h>
#include <ThunderDashboard/nt_handler.h>

namespace y2022 {

class ShooterPositionPage : public frc1511::Page {
public:
  static ShooterPositionPage* get() {
    return &instance;
  }

  ShooterPositionPage(ShooterPositionPage const&) = delete;
  ShooterPositionPage& operator=(ShooterPositionPage const&) = delete;

  void present(bool* running) override;

private:
  ShooterPositionPage();
  ~ShooterPositionPage();

  static ShooterPositionPage instance;
};

} // namespace y2022
