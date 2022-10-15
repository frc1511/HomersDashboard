#pragma once

#include <ThunderDashboard/pages/page.h>
#include <ThunderDashboard/nt_handler.h>

class MatchTimerPage : public frc1511::Page {
public:
  static MatchTimerPage* get() {
    return &instance;
  }

  MatchTimerPage(MatchTimerPage const&) = delete;
  MatchTimerPage& operator=(MatchTimerPage const&) = delete;

  void present(bool* running) override;

private:
  MatchTimerPage();
  ~MatchTimerPage();
  
  static MatchTimerPage instance;
};
