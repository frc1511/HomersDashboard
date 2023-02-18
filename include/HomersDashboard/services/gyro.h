#pragma once

#include <ThunderDashboard/nt_handler.h>
#include <ThunderDashboard/services/service.h>

class GyroService : public frc1511::Service {
public:
  static GyroService* get() {
    return &instance;
  }

  GyroService(GyroService const&) = delete;
  GyroService& operator=(GyroService const&) = delete;

  void init() override;
  void process() override;

private:
  GyroService();
  ~GyroService();

  bool blink = false;
  std::chrono::steady_clock::time_point blinkClock;
  ImVec4 bg_normal { 0.05f, 0.07f, 0.09f, 1.00f };
  ImVec4 bg_blink { 0.93f, 0.11f, 0.14f, 0.65f };

  static GyroService instance;
};
