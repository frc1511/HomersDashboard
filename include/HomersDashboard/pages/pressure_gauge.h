#pragma once

#include <ThunderDashboard/pages/page.h>

class PressureGaugePage : public frc1511::Page {
public:
  static PressureGaugePage* get() {
    return &instance;
  }

  PressureGaugePage(PressureGaugePage const&) = delete;
  PressureGaugePage& operator=(PressureGaugePage const&) = delete;
  
  void present(bool* running) override;

private:
  PressureGaugePage();
  ~PressureGaugePage();

  void show_pressure_gauge();

  static PressureGaugePage instance;
};