#include <HomersDashboard/graphics_manager.h>

//
// Default Window Properties.
//
#define WINDOW_WIDTH  1300
#define WINDOW_HEIGHT 800

#ifdef HD_WINDOWS // ------ DirectX 11 ------

#define WINDOW_TITLE L"1511 Dashboard"

#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

static UINT g_resize_width = 0, g_resize_height = 0;

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
                                                             UINT msg,
                                                             WPARAM wParam,
                                                             LPARAM lParam);

// Win32 message handler.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;

  switch (msg) {
  case WM_SIZE:
    if (wParam == SIZE_MINIMIZED) return 0;
    g_resize_width = (UINT)LOWORD(lParam); // Queue resize
    g_resize_height = (UINT)HIWORD(lParam);
    return 0;
  case WM_SYSCOMMAND:
    if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
      return 0;
    break;
  case WM_DESTROY:
    ::PostQuitMessage(0);
    return 0;
  }
  return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

#else // ------ OpenGL/GLFW ------

#define WINDOW_TITLE "1511 Dashboard"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

//
// GL/GLSL versions.
//
#if defined(IMGUI_IMPL_OPENGL_ES2)
#error "OpenGL ES 2 is not supported"
#endif

#ifdef HD_MACOS
#define GLSL_VERSION     "#version 150"
#define GL_VERSION_MAJOR 3
#define GL_VERSION_MINOR 2
#else
#define GLSL_VERSION     "#version 130"
#define GL_VERSION_MAJOR 3
#define GL_VERSION_MINOR 0
#endif

#endif

void GraphicsManager::init() {
#ifdef HD_WINDOWS
  // ImGui_ImplWin32_EnableDpiAwareness();
  m_wc = {sizeof(m_wc),
          CS_CLASSDC,
          WndProc,
          0L,
          0L,
          GetModuleHandle(nullptr),
          nullptr,
          nullptr,
          nullptr,
          nullptr,
          L"ThunderDashboard",
          nullptr};

  ::RegisterClassExW(&m_wc);

  m_hwnd = ::CreateWindowW(
      m_wc.lpszClassName, WINDOW_TITLE, WS_OVERLAPPEDWINDOW, 0, 0, WINDOW_WIDTH,
      WINDOW_HEIGHT, nullptr, nullptr, m_wc.hInstance, nullptr);

  // Initialize Direct3D
  if (!init_device()) {
    deinit_device();
    ::UnregisterClassW(m_wc.lpszClassName, m_wc.hInstance);
    exit(1);
  }

  // Show the window
  ::ShowWindow(m_hwnd, SW_SHOWDEFAULT);
  ::UpdateWindow(m_hwnd);

#else // ------ OpenGL/GLFW ------
  //
  // GLFW.
  //
  glfwSetErrorCallback([](int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
  });

  if (!glfwInit()) {
    exit(1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);

#if defined(HD_MACOS)
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
#endif

  // Initialize window.
  m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE,
                              nullptr, nullptr);
  if (!m_window) exit(1);

  glfwMakeContextCurrent(m_window);
  // VSync.
  glfwSwapInterval(true);

  //
  // Load OpenGL functions.
  //
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif

  //
  // Imgui.
  //
  IMGUI_CHECKVERSION();

  ImGui::CreateContext();

  {
    ImGuiIO* io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io->ConfigWindowsMoveFromTitleBarOnly = true;
  }

#ifdef HD_WINDOWS
  ImGui_ImplWin32_Init(m_hwnd);
  ImGui_ImplDX11_Init(m_device, m_device_context);
#else
  ImGui_ImplGlfw_InitForOpenGL(m_window, true);
  ImGui_ImplOpenGL3_Init(GLSL_VERSION);
#endif
}

bool GraphicsManager::poll_events() {
#ifdef HD_WINDOWS
  MSG msg;
  while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
    if (msg.message == WM_QUIT) return true;
  }

  // Handle window resize (we don't resize directly in the WM_SIZE handler)
  if (g_resize_width != 0 && g_resize_height != 0) {
    deinit_render_target();
    m_swap_chain->ResizeBuffers(0, g_resize_width, g_resize_height,
                                DXGI_FORMAT_UNKNOWN, 0);
    g_resize_width = g_resize_height = 0;
    init_render_target();
  }
  return false;
#else
  glfwPollEvents();
  return glfwWindowShouldClose(m_window);
#endif
}

void GraphicsManager::begin_frame() {
#ifdef HD_WINDOWS
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
#else
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
#endif

  ImGui::NewFrame();
}

void GraphicsManager::end_frame() {
  ImGui::Render();

  const ImVec4 clear_color = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

#ifdef HD_WINDOWS
  const float clear_color_with_alpha[4] = {
      clear_color.x * clear_color.w, clear_color.y * clear_color.w,
      clear_color.z * clear_color.w, clear_color.w};
  m_device_context->OMSetRenderTargets(1, &m_main_render_target_view, nullptr);
  m_device_context->ClearRenderTargetView(m_main_render_target_view,
                                          clear_color_with_alpha);
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

  // Update and Render additional Platform Windows
  if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
  }

  m_swap_chain->Present(1, 0); // Present with vsync

#else
  int buf_width, buf_height;
  glfwGetFramebufferSize(m_window, &buf_width, &buf_height);

  glViewport(0, 0, buf_width, buf_height);
  glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
               clear_color.z * clear_color.w, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }

  glfwSwapBuffers(m_window);
#endif
}

void GraphicsManager::deinit() {
#ifdef HD_WINDOWS
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
#else
  // Shutdown ImGui.
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
#endif

  ImGui::DestroyContext();

#ifdef HD_WINDOWS
  deinit_device();
  ::DestroyWindow(m_hwnd);
  ::UnregisterClassW(m_wc.lpszClassName, m_wc.hInstance);
#else
  // Shutdown GLFW.
  glfwDestroyWindow(m_window);
  glfwTerminate();
#endif
}

ImVec2 GraphicsManager::window_size() const {
#ifdef HD_WINDOWS
  return ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT);
#else
  int width, height;
  glfwGetWindowSize(m_window, &width, &height);
  return ImVec2(width, height);
#endif
}

ImVec2 GraphicsManager::window_pos() const {
#ifdef HD_WINDOWS
  return ImVec2(0, 0);
#else
  int x, y;
  glfwGetWindowPos(m_window, &x, &y);

  return ImVec2(x, y);
#endif
}

void GraphicsManager::set_window_size(int width, int height) {
#ifdef HD_WINDOWS
  (void)width;
  (void)height;
#else
  glfwSetWindowSize(m_window, width, height);
#endif
}

void GraphicsManager::set_window_pos(int x, int y) {
#ifdef HD_WINDOWS
  (void)x;
  (void)y;
#else
  glfwSetWindowPos(m_window, x, y);
#endif
}

#ifdef HD_WINDOWS // DirectX 11 helper functions.

bool GraphicsManager::init_device() {
  // Setup swap chain
  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory(&sd, sizeof(sd));
  sd.BufferCount = 2;
  sd.BufferDesc.Width = 0;
  sd.BufferDesc.Height = 0;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = m_hwnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;
  sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  UINT createDeviceFlags = 0;
  // createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
  D3D_FEATURE_LEVEL featureLevel;
  const D3D_FEATURE_LEVEL featureLevelArray[2] = {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_0,
  };
  HRESULT res = D3D11CreateDeviceAndSwapChain(
      nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
      featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &m_swap_chain, &m_device,
      &featureLevel, &m_device_context);
  if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software
                                     // driver if hardware is not available.
    res = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags,
        featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &m_swap_chain, &m_device,
        &featureLevel, &m_device_context);
  if (res != S_OK) return false;

  init_render_target();
  return true;
}

void GraphicsManager::deinit_device() {
  if (m_swap_chain) {
    m_swap_chain->Release();
    m_swap_chain = nullptr;
  }
  if (m_device_context) {
    m_device_context->Release();
    m_device_context = nullptr;
  }
  if (m_device) {
    m_device->Release();
    m_device = nullptr;
  }
}

void GraphicsManager::init_render_target() {
  ID3D11Texture2D* pBackBuffer;
  m_swap_chain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
  m_device->CreateRenderTargetView(pBackBuffer, nullptr,
                                   &m_main_render_target_view);
  pBackBuffer->Release();
}

void GraphicsManager::deinit_render_target() {
  if (m_main_render_target_view) {
    m_main_render_target_view->Release();
    m_main_render_target_view = nullptr;
  }
}

#endif
