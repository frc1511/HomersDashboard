#include <app.h>
#include <imgui_internal.h>
#include <IconsFontAwesome5.h>

#include <pages/auto_chooser.h>
#include <pages/motion_profile.h>
#include <pages/intake_camera.h>
#include <pages/limelight.h>
#include <pages/blinky_blinky.h>
#include <pages/settings.h>

App::App() { }

App::~App() { }

void App::init(GLFWwindow* win) {
    window = win;

    IntakeCameraPage::get()->init();
    LimelightPage::get()->init();
}

void App::present() {
  bool item_close = false;

  static bool show_network_tables = false,
              show_auto_chooser = false,
              show_motion_profile = false,
              show_intake_camera = false,
              show_limelight = false,
              show_blinky_blinky = false,
              show_settings = false;

  bool was_showing_intake_camera = show_intake_camera,
       was_showing_limelight = show_limelight;

#ifdef DASHBOARD_MACOS
# define CTRL_STR "Cmd+"
# define CTRL_SHIFT_STR "Cmd+Shift+"
#else
# define CTRL_STR "Ctrl+"
# define CTRL_SHIFT_STR "Ctrl+Shift+"
#endif
  
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      ImGui::MenuItem(ICON_FA_WINDOW_CLOSE "  Close", CTRL_STR "W", &item_close);
      
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Tools")) {
      ImGui::MenuItem(ICON_FA_CAMERA "  Intake Camera", nullptr, &show_intake_camera);
      ImGui::MenuItem(ICON_FA_LEMON "  Limelight", nullptr, &show_limelight);
      ImGui::MenuItem(ICON_FA_TH_LIST "  Network Tables", nullptr, &show_network_tables);
      ImGui::MenuItem(ICON_FA_BOLT "  Auto Chooser", nullptr, &show_auto_chooser);
      ImGui::MenuItem(ICON_FA_LIGHTBULB "  Blinky Blinky", nullptr, &show_blinky_blinky);

      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Diagnostics")) {
      ImGui::MenuItem(ICON_FA_CHART_LINE "  Motion Profile", nullptr, &show_motion_profile);

      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Settings")) {
      ImGui::MenuItem(ICON_FA_COG "  Settings", nullptr, &show_settings);

      ImGui::EndMenu();
    }
    
    ImGui::EndMenuBar();
  }

  if (item_close)      menu_close();

  if (show_auto_chooser) AutoChooserPage::get()->present(&show_auto_chooser);
  if (show_motion_profile) MotionProfilePage::get()->present(&show_motion_profile);
  if (show_intake_camera) IntakeCameraPage::get()->present(&show_intake_camera);
  if (show_limelight) LimelightPage::get()->present(&show_limelight);
  if (show_blinky_blinky) BlinkyBlinkyPage::get()->present(&show_blinky_blinky);
  if (show_settings) SettingsPage::get()->present(&show_settings);

  if (show_intake_camera != was_showing_intake_camera) {
    IntakeCameraPage::get()->set_running(show_intake_camera);
  }

  if (show_limelight != was_showing_limelight) {
    LimelightPage::get()->set_running(show_limelight);
  }

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
