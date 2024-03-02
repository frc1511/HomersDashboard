#pragma once

#include <HomersDashboard/homers_dashboard.h>

struct GLFWwindow;

class GraphicsManager { // singleton
  GraphicsManager() = default;

  GLFWwindow* m_window;

public:
  static GraphicsManager& get() {
    static GraphicsManager instance;
    return instance;
  }

  GraphicsManager(GraphicsManager const&) = delete;
  void operator=(GraphicsManager const&) = delete;

  void init();

  // Returns whether the window should close.
  bool poll_events();

  void begin_frame();
  void end_frame();

  // Opposite of init.
  void deinit();

  ImVec2 window_size() const;
  ImVec2 window_pos() const;

  void set_window_size(int width, int height);
  void set_window_pos(int x, int y);
};

