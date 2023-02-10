#pragma once

#include <ThunderDashboard/nt_handler.h>
#include <DualSenseWindows/Device.h>
#include <DualSenseWindows/DS5State.h>
#include <DualSenseWindows/Helpers.h>
#include <DualSenseWindows/IO.h>

extern "C" {
    struct InputState {
        uint32_t axisLeftX : 8;
        uint32_t axisLeftY : 8;
        uint32_t axisRightX : 8;
        uint32_t axisRightY : 8;
        uint32_t axisLeftTrigger : 8;
        uint32_t axisRightTrigger : 8;
        uint32_t buttonsAndDPad : 8;
        uint32_t buttonsA : 8;
        uint32_t accelX : 16;
        uint32_t accelY : 16;
        uint32_t accelZ : 16;
        uint32_t gyroX : 16;
        uint32_t gyroY : 16;
        uint32_t gyroZ : 16;
        uint32_t buttonsB : 8;
        uint32_t pad : 24;
    }; // 24 bytes
    typedef struct InputState InputState;

    struct OutputState {
        uint32_t rumbleLeft : 8;
        uint32_t rumbleRight : 8;
        uint32_t micLed : 2; // (0 = off, 1 = on, 2 = pulse)
        uint32_t playerLedFade : 1;
        uint32_t playerLedBitmask : 5; // (0x01 = left, 0x02 = middle left, 0x04 = middle, 0x08 = middle right, 0x10 = right)
        uint32_t lightbarR : 8;
        uint32_t lightbarG : 8;
        uint32_t lightbarB : 8;
        uint32_t leftTriggerStartPosition : 8;
        uint32_t leftTriggerEndPosition : 8;
        uint32_t leftTriggerForce : 8;
        uint32_t rightTriggerStartPosition : 8;
        uint32_t rightTriggerEndPosition : 8;
        uint32_t rightTriggerForce : 8;
        uint32_t leftTriggerEffect : 2; // (0 = off, 1 = section, 2 = continuous)
        uint32_t rightTriggerEffect : 2; // (0 = off, 1 = section, 2 = continuous)
        uint32_t pad : 28;
    }; // 16 bytes
    typedef struct OutputState OutputState;
}

class PS5ControllerHandler {
public:
  static PS5ControllerHandler* get() {
    return &instance;
  }

  PS5ControllerHandler(PS5ControllerHandler const&) = delete;
  PS5ControllerHandler& operator=(PS5ControllerHandler const&) = delete;

  void init();

  void process();

  bool reload_connections();

  void set_controller_ids(int driver, int aux);

  inline std::pair<int, int> get_controller_ids() const { return { driver_id, aux_id }; }

  constexpr void set_test_driver_controller(bool testing) { testing_driver = testing; }
  constexpr void set_test_aux_controller(bool testing) { testing_aux = testing; }

private:
  PS5ControllerHandler();
  ~PS5ControllerHandler();

  void thread_main(bool is_driver);

  void handle_controller_input(DS5W::DeviceContext& context, InputState* input);
  void handle_controller_output(DS5W::DeviceContext& context, const OutputState& ouput);

  int driver_id = 0;
  int aux_id = 1;

  DS5W::DeviceContext driver_context;
  DS5W::DeviceContext aux_context;

  std::thread driver_thread;
  std::thread aux_thread;

  InputState driver_input;
  InputState aux_input;

  bool new_driver_input = false;
  bool new_aux_input = false;

  std::mutex driver_input_mutex;
  std::mutex aux_input_mutex;

  OutputState driver_output;
  OutputState aux_output;

  bool new_driver_output = false;
  bool new_aux_output = false;

  std::mutex driver_output_mutex;
  std::mutex aux_output_mutex;

  bool valid_driver = false;
  bool valid_aux = false;

  bool has_init = false;

  bool testing_driver = false;
  bool testing_aux = false;

  static PS5ControllerHandler instance;
};
