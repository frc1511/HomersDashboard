#pragma once

#include <ThunderDashboard/nt_handler.h>
#include <ds5w.h>

extern "C" {
  struct PS5RumbleOptions {
    uint32_t left : 8;
    uint32_t right : 8;
    uint32_t pad : 16;
  };
  typedef struct PS5RumbleOptions PS5RumbleOptions;
  
  struct PS5TrigggerOptions {
    uint32_t start_position : 8;
    uint32_t end_position : 8;
    uint32_t force : 8;
    uint32_t trigger_effect : 2; // (0 = off, 1 = section, 2 = continuous)
    uint32_t pad : 6;
  };
  typedef struct PS5TriggerOptions PS5TriggerOptions;

  struct PS5LEDOptions {
    uint32_t lightbar_r : 8;
    uint32_t lightbar_g : 8;
    uint32_t lightbar_b : 8;
    uint32_t mic : 2; // (0 = off, 1 = on, 2 = pulse)
    uint32_t player_fade : 1;
    uint32_t player_bitmask : 5; // (0x01 = left, 0x02 = middle left, 0x04 = middle, 0x08 = middle right, 0x10 = right)
  };
  typedef struct PS5LEDOptions PS5LEDOptions;
}

class PS5ControllerHandler {
public:
  static PS5ControllerHandler* get() {
    return &instance;
  }

  PS5ControllerHandler(PS5ControllerHandler const&) = delete;
  PS5ControllerHandler& operator=(PS5ControllerHandler const&) = delete;

  struct Options {
    PS5RumbleOptions rumble;
    PS5TriggerOptions left_trigger;
    PS5TriggerOptions right_trigger;
    PS5LEDOptions leds;
  };

  void init();

  void process();

  bool reload_connections();

  void set_controller_id(int driver, int aux);

private:
  PS5ControllerHandler();
  ~PS5ControllerHandler();

  void handle_controller(DS5W::DeviceContext& context, const Options& options);

  int driver_id = 0;
  int aux_id = 1;

  DS5W::DeviceContext driver_context;
  DS5W::DeviceContext aux_context;

  Options driver_options;
  Options aux_options;

  bool valid_driver = false;
  bool valid_aux = false;

  bool has_init = false;

  static PS5ControllerHandler instance;
};
