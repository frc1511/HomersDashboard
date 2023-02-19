#include <HomersDashboard/services/2023/arduino.h>
#include <fmt/format.h>

using namespace y2023;

ArduinoService::ArduinoService() = default;

ArduinoService::~ArduinoService() {
  if (has_init) {
    CloseHandle(serial_handle);
  }
}

void ArduinoService::init() {
  serial_handle = CreateFile("\\\\.\\COM7", GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
  if (serial_handle <= 0) {
    std::cout << "CreateFile() failed D:\n";
  }

  // Do some basic settings
  DCB serial_params = { 0 };
  serial_params.DCBlength = sizeof(serial_params);

  GetCommState(serial_handle, &serial_params);
  serial_params.BaudRate = 9600;
  serial_params.ByteSize = 8;
  serial_params.StopBits = ONESTOPBIT;
  serial_params.Parity = NOPARITY;
  SetCommState(serial_handle, &serial_params);

  // Set timeouts
  COMMTIMEOUTS timeout = { 0 };
  timeout.ReadIntervalTimeout = 50;
  timeout.ReadTotalTimeoutConstant = 50;
  timeout.ReadTotalTimeoutMultiplier = 50;
  timeout.WriteTotalTimeoutConstant = 50;
  timeout.WriteTotalTimeoutMultiplier = 10;

  SetCommTimeouts(serial_handle, &timeout);

  com_thread = std::thread([this]() {
    thread_main();
  });

  has_init = true;
}

void ArduinoService::process() {
  frc1511::NTHandler::Alliance alliance = frc1511::NTHandler::get()->get_alliance();
  frc1511::NTHandler::MatchMode mode = frc1511::NTHandler::get()->get_match_mode();
  double period_time_left = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_match_remaining", 0.0);

  if (period_time_left == last_period_time) {
    return;
  }
  last_period_time = period_time_left;

  if (period_time_left < 0) {
    period_time_left = 0;
  }

  int time_left = 0;

  if (mode == frc1511::NTHandler::MatchMode::AUTO) {
    time_left = 150 - 15 + period_time_left;
  }
  else {
    time_left = period_time_left;
  }

  double match_pct = time_left / 150.0;

  // Structure: "<ALLIANCE,MATCH_PERCENT>

  std::string _output = fmt::format("<{},{}>", alliance == frc1511::NTHandler::Alliance::RED ? "R" : "B", match_pct);

  {
    std::lock_guard<std::mutex> lock(com_mutex);
    output = _output;
    new_output = true;
  }
}

void ArduinoService::thread_main() {
  char buf[32];
  std::size_t buf_size = 0;
  while (true) {
    {
      std::unique_lock<std::mutex> lk(com_mutex);
      if (!new_output) continue;

      buf_size = output.size();
      std::memcpy(buf, output.c_str(), buf_size + 1);
      new_output = false;
    }

    // Write to the serial port
    DWORD bytes_written = 0;
    WriteFile(serial_handle, buf, buf_size, &bytes_written, nullptr);
    std::cout << "Wrote " << bytes_written << " to Arduino\n";

  }
}

ArduinoService ArduinoService::instance;
