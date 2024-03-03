#pragma once

#include <HomersDashboard/homers_dashboard.h>

class GraphicsManager {
  GraphicsManager() = default;

#ifdef HD_WINDOWS
  ID3D11Device* m_device = nullptr;
  ID3D11DeviceContext* m_device_context = nullptr;
  IDXGISwapChain* m_swap_chain = nullptr;
  ID3D11RenderTargetView* m_main_render_target_view = nullptr;

  HWND m_hwnd;
  WNDCLASSEXW m_wc;
#else
  GLFWwindow* m_window;
#endif

public:
  static GraphicsManager& get() {
    static GraphicsManager instance;
    return instance;
  }

  GraphicsManager(GraphicsManager const&) = delete;
  void operator=(GraphicsManager const&) = delete;

#ifdef HD_WINDOWS
  ID3D11Device* device() { return m_device; }
  ID3D11DeviceContext* context() { return m_device_context; }
#endif

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

#ifdef HD_WINDOWS // DirectX 11 helper functions.
  bool init_device();
  void deinit_device();
  void init_render_target();
  void deinit_render_target();
#endif
};
