#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
// #include <platform/platform.h>
#include <dashboard.h>

class App {
public:
  static App* get() {
    return &instance;
  }
  
  App(App const&) = delete;
  App& operator=(App const&) = delete;

  void init(GLFWwindow* window);

  inline GLFWwindow* get_window() { return window; }
  
  void present();
  
  void menu_close();

  void close();
  
  void handle_keyboard(int key, int scancode, int action, int mods);
  
  constexpr bool is_running() const { return running; }

  unsigned get_page_states() const;
  void set_page_states(unsigned page_states);
  
private:
  App();
  ~App();

  enum class EventState {
    NONE = 0,
    CLOSE,
  };

  EventState event_state = EventState::NONE;
  
  bool running = true;
  
  GLFWwindow* window = nullptr;

  bool show_network_tables = false,
       show_robot_position = false,
       show_auto_chooser   = false,
       show_motion_profile = false,
       show_intake_camera  = false,
       show_limelight      = false,
       show_blinky_blinky  = false,
       show_settings       = false;

  enum Pages {
    PAGE_NETWORK_TABLES = 1 << 0,
    PAGE_ROBOT_POSITION = 1 << 1,
    PAGE_AUTO_CHOOSER   = 1 << 2,
    PAGE_MOTION_PROFILE = 1 << 3,
    PAGE_INTAKE_CAMERA  = 1 << 4,
    PAGE_LIMELIGHT      = 1 << 5,
    PAGE_BLINKY_BLINKY  = 1 << 6,
    PAGE_SETTINGS       = 1 << 7,
  };
  
  static App instance;
};
