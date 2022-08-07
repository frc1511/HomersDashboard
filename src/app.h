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
  
  static App instance;
};
