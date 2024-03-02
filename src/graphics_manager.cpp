#include <HomersDashboard/graphics_manager.h>

//
// Default Window Properties.
//
#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 700
#define WINDOW_TITLE  "1511 Auto Planner"

#ifdef HD_WINDOWS // ------ DirectX 11 ------

#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <tchar.h>

// Data
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

ID3D11Device* GraphicsManager::device() { return g_pd3dDevice; }

static HWND hwnd;
static WNDCLASSEXW wc;

//
// Helper functions (from ImGui Example).
//

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool CreateDeviceD3D(HWND hWnd) {
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
  sd.OutputWindow = hWnd;
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
      featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain,
      &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
  if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software
                                     // driver if hardware is not available.
    res = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags,
        featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain,
        &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
  if (res != S_OK) return false;

  CreateRenderTarget();
  return true;
}

void CleanupDeviceD3D() {
  CleanupRenderTarget();
  if (g_pSwapChain) {
    g_pSwapChain->Release();
    g_pSwapChain = nullptr;
  }
  if (g_pd3dDeviceContext) {
    g_pd3dDeviceContext->Release();
    g_pd3dDeviceContext = nullptr;
  }
  if (g_pd3dDevice) {
    g_pd3dDevice->Release();
    g_pd3dDevice = nullptr;
  }
}

void CreateRenderTarget() {
  ID3D11Texture2D* pBackBuffer;
  g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
  g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr,
                                       &g_mainRenderTargetView);
  pBackBuffer->Release();
}

void CleanupRenderTarget() {
  if (g_mainRenderTargetView) {
    g_mainRenderTargetView->Release();
    g_mainRenderTargetView = nullptr;
  }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
                                                             UINT msg,
                                                             WPARAM wParam,
                                                             LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;

  switch (msg) {
  case WM_SIZE:
    if (wParam == SIZE_MINIMIZED) return 0;
    g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
    g_ResizeHeight = (UINT)HIWORD(lParam);
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

static GLFWwindow* glfw_window;

#endif

void GraphicsManager::init() {

#ifdef HD_WINDOWS // ------ DirectX 11 ------
  // Create application window
  // ImGui_ImplWin32_EnableDpiAwareness();
  wc = {sizeof(wc),
        CS_CLASSDC,
        WndProc,
        0L,
        0L,
        GetModuleHandle(nullptr),
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        L"ImGui Example",
        nullptr};
  ::RegisterClassExW(&wc);
  hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example",
                         WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr,
                         nullptr, wc.hInstance, nullptr);

  // Initialize Direct3D
  if (!CreateDeviceD3D(hwnd)) {
    CleanupDeviceD3D();
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    exit(1);
  }

  // Show the window
  ::ShowWindow(hwnd, SW_SHOWDEFAULT);
  ::UpdateWindow(hwnd);

#else // ------ OpenGL/GLFW ------
  //
  // GLFW.
  //
  glfwSetErrorCallback([](int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
  });

  if (!glfwInit()) {
    exit(-1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);

#if defined(HD_MACOS) || defined(HD_TEST_MACOS)
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
#endif

  // Initialize window.
  glfw_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE,
                                 nullptr, nullptr);
  if (!glfw_window) exit(-1);

  glfwMakeContextCurrent(glfw_window);
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

#ifdef HD_WINDOWS // ------ DirectX 11 ------
  ImGui_ImplWin32_Init(hwnd);
  ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

#else // ------ OpenGL/GLFW ------
  ImGui_ImplGlfw_InitForOpenGL(glfw_window, true);
  ImGui_ImplOpenGL3_Init(GLSL_VERSION);
#endif
}

bool GraphicsManager::poll_events() {
#ifdef HD_WINDOWS // ------ DirectX 11 ------
  MSG msg;
  while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
    if (msg.message == WM_QUIT) return true;
  }

  // Handle window resize (we don't resize directly in the WM_SIZE handler)
  if (g_ResizeWidth != 0 && g_ResizeHeight != 0) {
    CleanupRenderTarget();
    g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight,
                                DXGI_FORMAT_UNKNOWN, 0);
    g_ResizeWidth = g_ResizeHeight = 0;
    CreateRenderTarget();
  }
  return false;
#else // ------ OpenGL/GLFW ------
  glfwPollEvents();
  return glfwWindowShouldClose(glfw_window);
#endif
}

void GraphicsManager::begin_frame() {
#ifdef HD_WINDOWS // ------ DirectX 11 ------
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
#else // ------ OpenGL/GLFW ------
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
#endif

  ImGui::NewFrame();
}

void GraphicsManager::end_frame() {
  ImGui::Render();
  ImVec4 clear_color = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

#ifdef HD_WINDOWS // ------ DirectX 11 ------
  const float clear_color_with_alpha[4] = {
      clear_color.x * clear_color.w, clear_color.y * clear_color.w,
      clear_color.z * clear_color.w, clear_color.w};
  g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
  g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView,
                                             clear_color_with_alpha);
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

  g_pSwapChain->Present(1, 0); // Present with vsync

#else // ------ OpenGL/GLFW ------
  int buf_width, buf_height;
  glfwGetFramebufferSize(glfw_window, &buf_width, &buf_height);

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

  glfwSwapBuffers(glfw_window);
#endif
}

void GraphicsManager::deinit() {
#ifdef HD_WINDOWS // ------ DirectX 11 ------
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
#else // ------ OpenGL/GLFW ------
  // Shutdown ImGui.
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
#endif

  ImGui::DestroyContext();

#ifdef HD_WINDOWS // ------ DirectX 11 ------
  CleanupDeviceD3D();
  ::DestroyWindow(hwnd);
  ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
#else // ------ OpenGL/GLFW ------
  // Shutdown GLFW.
  glfwDestroyWindow(glfw_window);
  glfwTerminate();
#endif
}

ImVec2 GraphicsManager::window_size() const {
#ifdef HD_WINDOWS // ------ DirectX 11 ------
  return ImVec2(640, 480);
#else // ------ OpenGL/GLFW ------
  int width, height;
  glfwGetWindowSize(glfw_window, &width, &height);
  return ImVec2(width, height);
#endif
}

ImVec2 GraphicsManager::window_pos() const {
#ifdef HD_WINDOWS // ------ DirectX 11 ------
  return ImVec2(0, 0);
#else // ------ OpenGL/GLFW ------
  int x, y;
  glfwGetWindowPos(glfw_window, &x, &y);

  return ImVec2(x, y);
#endif
}

void GraphicsManager::set_window_size(int width, int height) {
#ifdef HD_WINDOWS // ------ DirectX 11 ------
#else             // ------ OpenGL/GLFW ------
  glfwSetWindowSize(glfw_window, width, height);
#endif
}

void GraphicsManager::set_window_pos(int x, int y) {
#ifdef HD_WINDOWS // ------ DirectX 11 ------
#else             // ------ OpenGL/GLFW ------
  glfwSetWindowPos(glfw_window, x, y);
#endif
}

