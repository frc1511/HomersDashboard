#include <HomersDashboard/dashboard.h>
#include <imgui_internal.h>
#include <IconsFontAwesome5.h>
#include <GLFW/glfw3.h>

#include <ThunderDashboard/platform/platform.h>
#include <ThunderDashboard/nt_handler.h>
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
#include <HomersDashboard/pages/pressure_gauge.h>
#include <HomersDashboard/pages/2022/ball_count.h>
#include <HomersDashboard/pages/2022/shooter_position.h>
#include <HomersDashboard/pages/2023/node_selector.h>
#include <HomersDashboard/pages/2023/auto_config.h>
#include <HomersDashboard/pages/2023/auto_preview.h>
#include <HomersDashboard/pages/2023/lift.h>
#include <HomersDashboard/pages/2023/grabber.h>
#include <HomersDashboard/pages/2023/controllers.h>
#include <HomersDashboard/services/gyro.h>

#ifdef THUNDER_WINDOWS
# include <HomersDashboard/services/2023/arduino.h>
# include <HomersDashboard/ps5_contoller_handler.h>
#endif

namespace frc1511 {

ThunderDashboard* get_dashboard() {
  return HomersDashboard::get();
}

} // namespace frc1511

HomersDashboard::HomersDashboard()
: frc1511::ThunderDashboard("Homer"),
  all_pages({
    AutoChooserPage::get(),
    BlinkyBlinkyPage::get(),
    IntakeCameraPage::get(),
    LimelightPage::get(),
    MotionProfilePage::get(),
    NetworkTablesPage::get(),
    RobotPositionPage::get(),
    SettingsPage::get(),
    MatchTimerPage::get(),
    CompInfoPage::get(),
    PressureGaugePage::get(),
    y2022::BallCountPage::get(),
    y2022::ShooterPositionPage::get(),
    y2023::AutoConfigPage::get(),
    y2023::AutoPreviewPage::get(),
    y2023::NodeSelectorPage::get(),
    y2023::LiftPage::get(),
    y2023::GrabberPage::get(),
    y2023::ControllerPage::get(),
  }),
  all_services({
    GyroService::get(),
#ifdef THUNDER_WINDOWS
    y2023::ArduinoService::get(),
#endif
  }) { }

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

#ifdef THUNDER_WINDOWS
    PS5ControllerHandler::get()->init();
#endif

    for (frc1511::Page* page : all_pages) {
      page->init();
    }

    for (frc1511::Service* service : all_services) {
      service->init();
    }
  }

  frc1511::NTHandler::get()->update();

  for (frc1511::Service* service : all_services) {
    service->process();
  }

  bool item_close = false;

  bool was_showing_intake_camera = page_states & (1ull << 2),
       was_showing_limelight     = page_states & (1ull << 3);

#ifdef DASHBOARD_MACOS
# define CTRL_STR "Cmd+"
# define CTRL_SHIFT_STR "Cmd+Shift+"
#else
# define CTRL_STR "Ctrl+"
# define CTRL_SHIFT_STR "Ctrl+Shift+"
#endif

  auto menu_item = [&](const char* label, uint64_t page_index) {
    if (ImGui::MenuItem(label, nullptr, page_states & (1ull << page_index))) {
      page_states ^= (1ull << page_index);
    }
  };
  
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      ImGui::MenuItem(ICON_FA_WINDOW_CLOSE "  Close", CTRL_STR "W", &item_close);
      
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Tools")) {
      menu_item(ICON_FA_CAMERA         "  Intake Camera",     2);
      menu_item(ICON_FA_LEMON          "  Limelight",         3);
      menu_item(ICON_FA_TH_LIST        "  Network Tables",    5);
      menu_item(" " ICON_FA_BOLT       "   Auto Chooser",     0);
      menu_item(" " ICON_FA_LIGHTBULB  "   Blinky Blinky",    1);
      menu_item(ICON_FA_MAP_MARKED_ALT "  Robot Position",    6);
      menu_item(ICON_FA_CHART_LINE     "  Motion Profile",    4);
      menu_item(ICON_FA_CLOCK          "  Match Timer",       8);
      menu_item(" " ICON_FA_INFO       "   Competition Info", 9);
      menu_item(ICON_FA_SKULL          "  Pressure Gauge",    10);
      if (ImGui::BeginMenu("2022")) {
        menu_item(ICON_FA_BASEBALL_BALL "  Ball Count",       11);
        menu_item(ICON_FA_METEOR        "  Shooter Position", 12);
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("2023")) {
        menu_item(" " ICON_FA_BOLT       "   Auto Config",  13);
        menu_item(" " ICON_FA_CHART_LINE "  Auto Preview",  14);
        menu_item(" " ICON_FA_FILM       "  Lift",          16);
        menu_item(" " ICON_FA_FILM       "  Grabber",       17);
        menu_item(" " ICON_FA_FILM       "  Node Selector", 15);
        menu_item(" " ICON_FA_FILM       "  Controller Reference", 18);
        ImGui::EndMenu();
      }

      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Settings")) {
      menu_item(ICON_FA_COGS "  Settings", 7);

      ImGui::EndMenu();
    }
    
    ImGui::EndMenuBar();
  }

  if (item_close) close();

#ifdef THUNDER_WINDOWS
  PS5ControllerHandler::get()->process();
#endif

  for (int i = 0; i < all_pages.size(); i++) {
    bool showing_page = page_states & (1ull << i);
    bool was_showing_page = showing_page;
    if (showing_page) {
      all_pages.at(i)->present(&showing_page);
      page_states ^= (was_showing_page ^ showing_page) << i; // xor is awful
    }
  }

  bool showing_intake_camera = page_states & (1ull << 2);
  if (was_showing_intake_camera != showing_intake_camera) {
    IntakeCameraPage::get()->set_running(showing_intake_camera);
  }

  bool showing_limelight = page_states & (1ull << 3);
  if (was_showing_limelight != showing_limelight) {
    LimelightPage::get()->set_running(showing_limelight);
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

void HomersDashboard::update_page_states() {
  IntakeCameraPage::get()->set_running(page_states & (1ull << 2));
  LimelightPage::get()->set_running(page_states & (1ull << 3));
}

// --- User Data ---

void HomersDashboard::data_clear() {
  data_map.clear();
}

bool HomersDashboard::data_should_open(const char* name) {
  app_section = std::string_view(name) == "App_State";
  if (app_section) return true;

  current_page = std::find_if(all_pages.cbegin(), all_pages.cend(), [&](const auto& page) {
    return std::string_view(page->get_save_name()) == name;
  });

  return current_page != all_pages.cend();
}

void HomersDashboard::data_read_line(const char* _line) {
  std::string line = _line;
  std::size_t del = line.find("=");
  std::string section_name = app_section ? "App_State" : (*current_page)->get_save_name();
  data_map[section_name][line.substr(0, del)] = line.substr(del + 1, line.size());
}

void HomersDashboard::data_apply(const char* type_name) {
  for (const auto& [section, data] : data_map) {
    if (section == "App_State") {
      for (const auto& [key, value] : data) {
        // Whether pages are open or not.
        if (key == "Page_States") {
          page_states = std::atoll(value.c_str());
          update_page_states();
        }

        // Window dimensions.
        int width, height;
        glfwGetWindowSize(get_window(), &width, &height);

        if (key == "Window_Width") {
          glfwSetWindowSize(get_window(), std::atoi(value.c_str()), height);
        }
        if (key == "Window_Height") {
          glfwSetWindowSize(get_window(), width, std::atoi(value.c_str()));
        }

        // Window position.
        int xpos, ypos;
        glfwGetWindowPos(get_window(), &xpos, &ypos);

        if (key == "Window_XPos") {
          glfwSetWindowPos(get_window(), std::atoi(value.c_str()), ypos);
        }
        if (key == "Window_YPos") {
          glfwSetWindowPos(get_window(), xpos, std::atoi(value.c_str()));
        }
      }
    }
    else {
      auto page = std::find_if(all_pages.cbegin(), all_pages.cend(), [&section = section](const auto& page) {
        return std::string_view(page->get_save_name()) == section;
      });
      if (page != all_pages.cend()) {
        (*page)->apply_save_data(data);
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

  begin_section("App_State");
  buf->appendf("Page_States=%llu\n", page_states);
  int width, height;
  glfwGetWindowSize(get_window(), &width, &height);
  buf->appendf("Window_Width=%d\n", width);
  buf->appendf("Window_Height=%d\n", height);
  int xpos, ypos;
  glfwGetWindowPos(get_window(), &xpos, &ypos);
  buf->appendf("Window_XPos=%d\n", xpos);
  buf->appendf("Window_YPos=%d\n", ypos);
  end_section();

  std::string_view save_name;
  std::map<std::string, std::string> write_data;
  for (auto& page : all_pages) {
    write_data = page->get_save_data();
    save_name = page->get_save_name();
    if (write_data.empty() || save_name.empty()) continue;

    begin_section(save_name.data());
    for (auto& [key, val] : write_data) {
      buf->appendf("%s=%s\n", key.c_str(), val.c_str());
    }
    end_section();
  }
}

HomersDashboard HomersDashboard::instance;
