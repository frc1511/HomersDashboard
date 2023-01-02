#include <HomersDashboard/dashboard.h>
#include <imgui_internal.h>
#include <IconsFontAwesome5.h>
#include <GLFW/glfw3.h>

#include <ThunderDashboard/nt_handler.h>
#include <HomersDashboard/gyro.h>
#include <HomersDashboard/popups/startup.h>
#include <HomersDashboard/pages/auto_chooser.h>
#include <HomersDashboard/pages/robot_position.h>
#include <HomersDashboard/pages/motion_profile.h>
#include <HomersDashboard/pages/intake_camera.h>
#include <HomersDashboard/pages/limelight.h>
#include <HomersDashboard/pages/network_tables.h>
#include <HomersDashboard/pages/blinky_blinky.h>
#include <HomersDashboard/pages/settings.h>
#include <HomersDashboard/pages/match_timer.h>
#include <HomersDashboard/pages/comp_info.h>
#include <HomersDashboard/pages/2022/ball_count.h>
#include <HomersDashboard/pages/2022/shooter_position.h>

namespace frc1511 {

ThunderDashboard* get_dashboard() {
  return HomersDashboard::get();
}

} // namespace frc1511

HomersDashboard::HomersDashboard()
: frc1511::ThunderDashboard("Homer") { }

HomersDashboard::~HomersDashboard() = default;

void HomersDashboard::init() { }

void HomersDashboard::present() {
  auto handle_state = [&]() {
    switch (event_state) {
      case EventState::NONE:
        break;
      case EventState::CLOSE:
        running = false;
        break;
    }
  };

  if (!initialized) {
    ImGui::OpenPopup(StartupPopup::get()->get_name().c_str());

    bool showing_popup = true;
    StartupPopup::get()->present(&showing_popup);

    if (showing_popup) {
      handle_state();
      return;
    }

    initialized = true;

    StartupPopup::Result result = StartupPopup::get()->get_result();

    frc1511::NTHandler::get()->init(result.version, result.ds_running);

    AutoChooserPage::get()->init();
    BlinkyBlinkyPage::get()->init();
    IntakeCameraPage::get()->init();
    LimelightPage::get()->init();
    MotionProfilePage::get()->init();
    NetworkTablesPage::get()->init();
    RobotPositionPage::get()->init();
    SettingsPage::get()->init();
    MatchTimerPage::get()->init();
    CompInfoPage::get()->init();
    BallCountPage::get()->init();
    ShooterPositionPage::get()->init();
  }

  frc1511::NTHandler::get()->update();

  GyroHandler::get()->handle_calibration();

  bool item_close = false;

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
      ImGui::MenuItem(ICON_FA_CAMERA         "  Intake Camera",  nullptr, &show_intake_camera);
      ImGui::MenuItem(ICON_FA_LEMON          "  Limelight",      nullptr, &show_limelight);
      ImGui::MenuItem(ICON_FA_TH_LIST        "  Network Tables", nullptr, &show_network_tables);
      ImGui::MenuItem(" " ICON_FA_BOLT       "   Auto Chooser",  nullptr, &show_auto_chooser);
      ImGui::MenuItem(" " ICON_FA_LIGHTBULB  "   Blinky Blinky", nullptr, &show_blinky_blinky);
      ImGui::MenuItem(ICON_FA_MAP_MARKED_ALT "  Robot Position", nullptr, &show_robot_position);
      ImGui::MenuItem(ICON_FA_CHART_LINE     "  Motion Profile", nullptr, &show_motion_profile);
      ImGui::MenuItem(ICON_FA_CLOCK          "  Match Timer",    nullptr, &show_match_timer);
      ImGui::MenuItem(" " ICON_FA_INFO           "   Competition Info", nullptr, &show_comp_info);
      if (ImGui::BeginMenu("2022")) {
        ImGui::MenuItem(ICON_FA_BASEBALL_BALL "  Ball Count",    nullptr, &show_2022_ball_count);
        ImGui::MenuItem(ICON_FA_METEOR "  Shooter Position",    nullptr, &show_2022_shooter_pos);
        ImGui::EndMenu();
      }

      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Settings")) {
      ImGui::MenuItem(ICON_FA_COG "  Settings", nullptr, &show_settings);

      ImGui::EndMenu();
    }
    
    ImGui::EndMenuBar();
  }

  if (item_close) close();

  if (show_settings) SettingsPage::get()->present(&show_settings);
  if (show_blinky_blinky) BlinkyBlinkyPage::get()->present(&show_blinky_blinky);
  if (show_auto_chooser) AutoChooserPage::get()->present(&show_auto_chooser);
  if (show_intake_camera) IntakeCameraPage::get()->present(&show_intake_camera);
  if (show_limelight) LimelightPage::get()->present(&show_limelight);
  if (show_network_tables) NetworkTablesPage::get()->present(&show_network_tables);
  if (show_motion_profile) MotionProfilePage::get()->present(&show_motion_profile);
  if (show_robot_position) RobotPositionPage::get()->present(&show_robot_position);
  if (show_match_timer) MatchTimerPage::get()->present(&show_match_timer);
  if (show_comp_info) CompInfoPage::get()->present(&show_comp_info);
  if (show_2022_ball_count) BallCountPage::get()->present(&show_2022_ball_count);
  if (show_2022_shooter_pos) ShooterPositionPage::get()->present(&show_2022_shooter_pos);

  if (show_intake_camera != was_showing_intake_camera) {
    IntakeCameraPage::get()->set_running(show_intake_camera);
  }

  if (show_limelight != was_showing_limelight) {
    LimelightPage::get()->set_running(show_limelight);
  }

  handle_state();
}

void HomersDashboard::close() {
  event_state = EventState::CLOSE;
}

void HomersDashboard::handle_keyboard(int key, int scancode, int action, int mods) {
  (void)scancode;

#ifdef DASHBOARD_MACOS
  int ctrl_key = GLFW_MOD_SUPER;
#else
  int ctrl_key = GLFW_MOD_CONTROL;
#endif
  
#define GET_CTRL_KEY(k)       ((key == k) && (mods == ctrl_key) && (action == GLFW_PRESS))
#define GET_CTRL_SHIFT_KEY(k) ((key == k) && (mods == (ctrl_key | GLFW_MOD_SHIFT)) && (action == GLFW_PRESS))
  
  if (GET_CTRL_KEY(GLFW_KEY_W)) {
    close();
  }
}

unsigned HomersDashboard::get_page_states() const {
  unsigned states = 0;

  if (show_network_tables) states |= PAGE_NETWORK_TABLES;
  if (show_robot_position) states |= PAGE_ROBOT_POSITION;
  if (show_auto_chooser)   states |= PAGE_AUTO_CHOOSER;
  if (show_motion_profile) states |= PAGE_MOTION_PROFILE;
  if (show_intake_camera)  states |= PAGE_INTAKE_CAMERA;
  if (show_limelight)      states |= PAGE_LIMELIGHT;
  if (show_blinky_blinky)  states |= PAGE_BLINKY_BLINKY;
  if (show_settings)       states |= PAGE_SETTINGS;
  if (show_match_timer)    states |= PAGE_MATCH_TIMER;
  if (show_comp_info)      states |= PAGE_COMP_INFO;
  if (show_2022_ball_count) states |= PAGE_2022_BALL_COUNT;
  if (show_2022_shooter_pos) states |= PAGE_2022_SHOOTER_POS;

  return states;
}

void HomersDashboard::set_page_states(unsigned states) {
  show_network_tables = states & PAGE_NETWORK_TABLES;
  show_robot_position = states & PAGE_ROBOT_POSITION;
  show_auto_chooser   = states & PAGE_AUTO_CHOOSER;
  show_motion_profile = states & PAGE_MOTION_PROFILE;
  show_intake_camera  = states & PAGE_INTAKE_CAMERA;
  show_limelight      = states & PAGE_LIMELIGHT;
  show_blinky_blinky  = states & PAGE_BLINKY_BLINKY;
  show_settings       = states & PAGE_SETTINGS;
  show_match_timer    = states & PAGE_MATCH_TIMER;
  show_comp_info      = states & PAGE_COMP_INFO;
  show_2022_ball_count = states & PAGE_2022_BALL_COUNT;
  show_2022_shooter_pos = states & PAGE_2022_SHOOTER_POS;

  IntakeCameraPage::get()->set_running(show_intake_camera);
  LimelightPage::get()->set_running(show_limelight);
}

// --- User Data ---

#define SECTION_APP_STATE "AppState"
#define KEY_PAGE_STATES "PageStates"

#define SECTION_SETTINGS "Settings"
#define KEY_TEAM_NUMBER "TeamNumber"

#define SECTION_AUTO_CHOOSER "AutoChooser"
#define KEY_AUTO_MODE "AutoMode"
#define KEY_AUTO_DELAY "AutoDelay"

#define SECTION_BLINKY_BLINKY "BlinkyBlinky"
#define KEY_LED_MODE "LedMode"
#define LED_CUSTOM_R "CustomR"
#define LED_CUSTOM_G "CustomG"
#define LED_CUSTOM_B "CustomB"

#define SECTION_WINDOW_STUFF "WindowSize"
#define KEY_WIN_WIDTH "Width"
#define KEY_WIN_HEIGHT "Height"
#define KEY_WIN_XPOS "XPos"
#define KEY_WIN_YPOS "YPos"

void HomersDashboard::data_clear() {
  data_map.clear();
}

bool HomersDashboard::data_should_open(const char* name) {
  bool res = true;

  if (std::strcmp(name, SECTION_APP_STATE) && std::strcmp(name, SECTION_SETTINGS) && std::strcmp(name, SECTION_AUTO_CHOOSER) && std::strcmp(name, SECTION_BLINKY_BLINKY) && std::strcmp(name, SECTION_WINDOW_STUFF)) {
    res = false;
  }

  return res;
}

void HomersDashboard::data_read_line(const char* line) {
  data_map.insert(line);
}

void HomersDashboard::data_apply(const char* type_name) {
  std::size_t del;
  std::string key;
  std::string val;
  for (auto& line : data_map) {
    del = line.find("=");
    key = line.substr(0, del);
    val = line.substr(del + 1, line.size());
    
    if (std::strcmp(type_name, get_data_name()) == 0) {
      if (key == KEY_PAGE_STATES) {
        set_page_states(std::atoll(val.c_str()));
      }
      if (key == KEY_TEAM_NUMBER) {
        SettingsPage::get()->set_team_number(std::atol(val.c_str()));
      }
      if (key == KEY_AUTO_MODE) {
        AutoChooserPage::get()->set_auto_mode(std::atoi(val.c_str()));
      }
      if (key == KEY_AUTO_DELAY) {
        AutoChooserPage::get()->set_auto_delay(std::atof(val.c_str()));
      }
      if (key == KEY_LED_MODE) {
        BlinkyBlinkyPage::get()->set_led_mode(std::atoi(val.c_str()));
      }
      if (key == LED_CUSTOM_R) {
        BlinkyBlinkyPage::get()->set_custom_r(std::atof(val.c_str()));
      }
      if (key == LED_CUSTOM_G) {
        BlinkyBlinkyPage::get()->set_custom_g(std::atof(val.c_str()));
      }
      if (key == LED_CUSTOM_B) {
        BlinkyBlinkyPage::get()->set_custom_b(std::atof(val.c_str()));
      }

      int width, height;
      glfwGetWindowSize(get_window(), &width, &height);

      if (key == KEY_WIN_WIDTH) {
        glfwSetWindowSize(get_window(), std::atoi(val.c_str()), height);
      }
      if (key == KEY_WIN_HEIGHT) {
        glfwSetWindowSize(get_window(), width, std::atoi(val.c_str()));
      }

      int xpos, ypos;
      glfwGetWindowPos(get_window(), &xpos, &ypos);

      if (key == KEY_WIN_XPOS) {
        glfwSetWindowPos(get_window(), std::atoi(val.c_str()), ypos);
      }
      if (key == KEY_WIN_YPOS) {
        glfwSetWindowPos(get_window(), xpos, std::atoi(val.c_str()));
      }
    }
  }
}

void HomersDashboard::data_write(const char* type_name, ImGuiTextBuffer* buf) {
    auto begin_section = [&](const char* name) {
    buf->appendf("[%s][%s]\n", type_name, name);
  };
  
  auto end_section = [&]() {
    buf->append("\n");
  };
  
  auto add_entry = [&](const char* key, const char* val_format, auto ...args) {
    std::string format(std::string("%s=") + val_format + '\n');
    buf->appendf(format.c_str(), key, args...);
  };
  
  if (std::strcmp(type_name, get_data_name()) == 0) {
    begin_section(SECTION_APP_STATE);
    add_entry(KEY_PAGE_STATES, "%lld", get_page_states());
    end_section();

    begin_section(SECTION_SETTINGS);
    add_entry(KEY_TEAM_NUMBER, "%lu", SettingsPage::get()->get_team_number());
    end_section();

    begin_section(SECTION_AUTO_CHOOSER);
    add_entry(KEY_AUTO_MODE, "%d", AutoChooserPage::get()->get_auto_mode());
    add_entry(KEY_AUTO_DELAY, "%f", AutoChooserPage::get()->get_auto_delay());
    end_section();

    begin_section(SECTION_BLINKY_BLINKY);
    add_entry(KEY_LED_MODE, "%d", BlinkyBlinkyPage::get()->get_led_mode());
    auto color(BlinkyBlinkyPage::get()->get_custom_color());
    add_entry(LED_CUSTOM_R, "%f", std::get<0>(color));
    add_entry(LED_CUSTOM_G, "%f", std::get<1>(color));
    add_entry(LED_CUSTOM_B, "%f", std::get<2>(color));
    end_section();

    begin_section(SECTION_WINDOW_STUFF);
    int width, height;
    glfwGetWindowSize(get_window(), &width, &height);
    add_entry(KEY_WIN_WIDTH, "%d", width);
    add_entry(KEY_WIN_HEIGHT, "%d", height);
    int xpos, ypos;
    glfwGetWindowPos(get_window(), &xpos, &ypos);
    add_entry(KEY_WIN_XPOS, "%d", xpos);
    add_entry(KEY_WIN_YPOS, "%d", ypos);
    end_section();
  }
}

HomersDashboard HomersDashboard::instance;
