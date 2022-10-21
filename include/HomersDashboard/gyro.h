#pragma once

#include <ThunderDashboard/nt_handler.h>

class GyroHandler {
public:
  static GyroHandler* get() {
    return &instance;
  }

  GyroHandler(GyroHandler const&) = delete;
  GyroHandler& operator=(GyroHandler const&) = delete;

  void handle_calibration();

private:
  GyroHandler();
  ~GyroHandler();

  bool blink = false;
  std::chrono::steady_clock::time_point blinkClock;
  ImVec4 bg_normal { 0.05f, 0.07f, 0.09f, 1.00f };
  ImVec4 bg_blink { 0.93f, 0.11f, 0.14f, 0.65f };

  static GyroHandler instance;
};
