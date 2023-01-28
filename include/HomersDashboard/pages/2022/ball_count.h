#pragma once

#include <ThunderDashboard/pages/page.h>
#include <ThunderDashboard/nt_handler.h>

namespace y2022 {

class BallCountPage : public frc1511::Page {
public:
  static BallCountPage* get() {
    return &instance;
  }

  BallCountPage(BallCountPage const&) = delete;
  BallCountPage& operator=(BallCountPage const&) = delete;

  void init() override;
  void present(bool* running) override;

private:
  BallCountPage();
  ~BallCountPage();

  unsigned int no_cargo_tex, red_cargo_tex, blue_cargo_tex;
  
  static BallCountPage instance;
};

}  // namespace y2022
