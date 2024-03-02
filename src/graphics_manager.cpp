#include <HomersDashboard/graphics_manager.h>

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

//
// Default Window Properties.
//
#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 700
#define WINDOW_TITLE  "1511 Auto Planner"

void GraphicsManager::init() {
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
  m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE,
                              nullptr, nullptr);
  if (!m_window) exit(-1);

  glfwMakeContextCurrent(m_window);
  // VSync.
  glfwSwapInterval(true);

  //
  // Load OpenGL functions.
  //
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

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

  // Setup OpenGL backend.
  ImGui_ImplGlfw_InitForOpenGL(m_window, true);
  ImGui_ImplOpenGL3_Init(GLSL_VERSION);
}

bool GraphicsManager::poll_events() {
  glfwPollEvents();
  return glfwWindowShouldClose(m_window);
}

void GraphicsManager::begin_frame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();

  ImGui::NewFrame();
}

void GraphicsManager::end_frame() {
  ImGui::Render();

  int buf_width, buf_height;
  glfwGetFramebufferSize(m_window, &buf_width, &buf_height);

  glViewport(0, 0, buf_width, buf_height);
  ImVec4 clear_color = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
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
}

void GraphicsManager::deinit() {
  // Shutdown ImGui.
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  // Shutdown GLFW.
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

ImVec2 GraphicsManager::window_size() const {
  int width, height;
  glfwGetWindowSize(m_window, &width, &height);

  return ImVec2(width, height);
}

ImVec2 GraphicsManager::window_pos() const {
  int x, y;
  glfwGetWindowPos(m_window, &x, &y);

  return ImVec2(x, y);
}

void GraphicsManager::set_window_size(int width, int height) {
  glfwSetWindowSize(m_window, width, height);
}

void GraphicsManager::set_window_pos(int x, int y) {
  glfwSetWindowPos(m_window, x, y);
}

