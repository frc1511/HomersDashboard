#include <app.h>
#include <imgui_internal.h>
#include <IconsFontAwesome5.h>

App::App() { }

App::~App() { }

void App::init(GLFWwindow* win) {
    window = win;
}

void App::present() {
  bool item_close = false;

#ifdef DASHBOARD_MACOS
# define CTRL_STR "Cmd+"
# define CTRL_SHIFT_STR "Cmd+Shift+"
#else
# define CTRL_STR "Ctrl+"
# define CTRL_SHIFT_STR "Ctrl+Shift+"
#endif
  
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      ImGui::MenuItem(ICON_FA_WINDOW_CLOSE "  Close",   CTRL_STR "W",       &item_close);
      
      ImGui::EndMenu();
    }
    
    ImGui::EndMenuBar();
  }

  if (item_close)      menu_close();

  switch (event_state) {
    case EventState::NONE:
      break;
    case EventState::CLOSE:
      running = false;
      break;
  }
}

void App::menu_close() {
  event_state = EventState::CLOSE;
}

void App::close() {
  event_state = EventState::CLOSE;
}

void App::handle_keyboard(int key, int scancode, int action, int mods) {
#ifdef DASHBOARD_MACOS
  int ctrl_key = GLFW_MOD_SUPER;
#else
  int ctrl_key = GLFW_MOD_CONTROL;
#endif
  
#define GET_CTRL_KEY(k)       ((key == k) && (mods == ctrl_key) && (action == GLFW_PRESS))
#define GET_CTRL_SHIFT_KEY(k) ((key == k) && (mods == (ctrl_key | GLFW_MOD_SHIFT)) && (action == GLFW_PRESS))
  
  if (GET_CTRL_KEY(GLFW_KEY_W)) {
    menu_close();
  }
}

App App::instance {};
