#pragma once

#include <ThunderDashboard/nt_handler.h>
#include <ThunderDashboard/services/service.h>
#include <thread>
#include <mutex>
#include <Windows.h>

namespace y2023 {

class ArduinoService : public frc1511::Service {
public:
  static ArduinoService* get() {
    return &instance;
  }
  
  ArduinoService(ArduinoService const&) = delete;
  ArduinoService& operator=(ArduinoService const&) = delete;
  
  void init() override;
  void process() override;

private:
  ArduinoService();
  ~ArduinoService();

  void thread_main();

  double last_period_time = -1.0;

  std::mutex com_mutex;
  std::thread com_thread;

  bool new_output = false;
  std::string output;

  HANDLE serial_handle;

  bool has_init = false;

  static ArduinoService instance;
};

} // namespace y2023
