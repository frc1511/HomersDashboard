#pragma once

#include <ThunderDashboard/nt_handler.h>
#include <ds5w.h>

extern "C" {
  struct RumbleOptions {
    uint32_t rumble_left : 8;
    uint32_t rumble_right : 8;
    uint32_t pad : 16;
  };
  typedef struct RumbleOptions RumbleOptions;
  
  struct TrigggerOptions {
    uint32_t trigger_effect : 2; // (0 = off, 1 = section, 2 = continuous)
    uint32_t start_position : 8;
    uint32_t end_position : 8;
    uint32_t force : 8;
    uint32_t pad : 6;
  };
  typedef struct TriggerOptions TriggerOptions;

  struct LEDOptions {
    unsigned char led_r;
    unsigned char led_g;
    unsigned char led_b;
    uint32_t mic_led : 2; // (0 = off, 1 = on, 2 = blink)
    uint32_t pad : 6;
  };
  typedef struct LEDOptions LEDOptions;
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

  void set_controller_id(int driver, int aux);

private:
  PS5ControllerHandler();
  ~PS5ControllerHandler();

  int driver_id = 0;
  int aux_id = 1;

  DS5W::DeviceContext driver_context;
  DS5W::DeviceContext aux_context;

  bool valid_driver = false;
  bool valid_aux = false;

  bool has_init = false;

  static PS5ControllerHandler instance;
};
