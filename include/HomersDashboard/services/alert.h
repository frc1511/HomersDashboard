#pragma once

#include <ThunderDashboard/nt_handler.h>
#include <ThunderDashboard/services/service.h>

class AlertService : public frc1511::Service {
public:
  static AlertService* get() {
    return &instance;
  }

  AlertService(AlertService const&) = delete;
  AlertService& operator=(AlertService const&) = delete;

  void init() override;
  void process() override;

private:
  AlertService();
  ~AlertService();

  bool blink = false;
  std::chrono::steady_clock::time_point blinkClock;
  ImVec4 bg_normal { 0.05f, 0.07f, 0.09f, 1.00f };
  ImVec4 lift_broken_kinda_bg { 1.00f, 1.00f, 0.00f, 1.00f }; // Yellow
  ImVec4 lift_broken_a_lot_bg { 1.00f, 0.34f, 0.00f, 1.00f }; // Orange
  ImVec4 gyro_bg { 0.93f, 0.11f, 0.14f, 0.65f }; // Red

  static AlertService instance;
};
