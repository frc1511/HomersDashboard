#pragma once

#include <HomersDashboard/font_library.h>
#include <HomersDashboard/homers_dashboard.h>
#include <HomersDashboard/nt_handler.h>

#include <HomersDashboard/services/alert_service.h>

#include <HomersDashboard/popups/startup_popup.h>

#include <HomersDashboard/pages/autonomous_chooser_page.h>
#include <HomersDashboard/pages/competition_info_page.h>
#include <HomersDashboard/pages/intake_camera_page.h>
#include <HomersDashboard/pages/limelight_camera_page.h>
#include <HomersDashboard/pages/match_timer_page.h>
#include <HomersDashboard/pages/network_tables_page.h>
#include <HomersDashboard/pages/robot_position_page.h>

#include <HomersDashboard/pages/2022/ball_count_page.h>
#include <HomersDashboard/pages/2022/shooter_position_page.h>

#include <HomersDashboard/pages/2023/controllers_page.h>
#include <HomersDashboard/pages/2023/grabber_page.h>
#include <HomersDashboard/pages/2023/lift_page.h>

#include <HomersDashboard/pages/2024/arm_page.h>
#include <HomersDashboard/pages/2024/shooter_page.h>

struct GLFWwindow;

class Page;

class App {
  GLFWwindow* m_window;
  FontLibrary& m_font_lib;

  NTHandler m_nt_handler;

  bool m_init = false;
  bool m_running = true;

  // The state of the application (used to manage popups and stuff).
  enum class EventState {
    NONE = 0,
    STARTUP,
    CLOSE,
  };

  EventState m_event_state = EventState::STARTUP;

  //
  // Services.
  //
  AlertService m_alert_service {m_nt_handler};

  const std::set<Service*> m_services {
      &m_alert_service,
  };

  //
  // Popups.
  //

  StartupPopup m_startup_popup;

  //
  // Pages.
  //

  CompetitionInfoPage m_comp_info_page {m_font_lib, m_nt_handler};
  MatchTimerPage m_match_timer_page {m_font_lib, m_nt_handler};
  NetworkTablesPage m_network_tables_page {m_font_lib, m_nt_handler};
  AutonomousChooserPage m_auto_chooser_page {m_nt_handler};
  RobotPositionPage m_robot_position_page {m_nt_handler};
  IntakeCameraPage m_intake_camera_page {};
  LimelightCameraPage m_limelight_camera_page {};

  const std::set<Page*> m_general_pages {
      &m_comp_info_page,        &m_match_timer_page,    &m_network_tables_page,
      &m_auto_chooser_page,     &m_robot_position_page, &m_intake_camera_page,
      &m_limelight_camera_page,
  };

  y2022::BallCountPage m_2022_ball_count_page {m_nt_handler};
  y2022::ShooterPositionPage m_2022_shooter_position_page {m_nt_handler};

  const std::set<Page*> m_2022_pages {
      &m_2022_ball_count_page,
      &m_2022_shooter_position_page,
  };

  y2023::LiftPage m_2023_lift_page {m_nt_handler};
  y2023::GrabberPage m_2023_grabber_page {m_nt_handler};
  y2023::ControllerPage m_2023_controller_page {};

  const std::set<Page*> m_2023_pages {
      &m_2023_lift_page,
      &m_2023_grabber_page,
      &m_2023_controller_page,
  };

  y2024::ArmPage m_2024_arm_page {m_nt_handler};

  const std::set<Page*> m_2024_pages {
      &m_2024_arm_page,
  };

  // Every page goes in here.
  const std::set<Page*> m_all_pages {
      &m_comp_info_page,       &m_match_timer_page,
      &m_network_tables_page,  &m_auto_chooser_page,
      &m_intake_camera_page,   &m_limelight_camera_page,
      &m_robot_position_page,

      &m_2022_ball_count_page, &m_2022_shooter_position_page,

      &m_2023_lift_page,       &m_2023_grabber_page,
      &m_2023_controller_page,

      &m_2024_arm_page,
  };

  //
  // Save data.
  //
  using DataLines = std::map<std::string, std::string>;

  DataLines m_window_data;
  std::map<Page*, DataLines> m_page_data;

  enum class SaveSection {
    NONE,
    WINDOW_STATE,
    PAGE_DATA,
  };
  SaveSection m_save_section = SaveSection::NONE;
  Page* m_save_page = nullptr;

public:
  inline App(GLFWwindow* window, FontLibrary& font_lib)
    : m_window(window),
      m_font_lib(font_lib) {}

  ~App() = default;

  constexpr bool is_running() const { return m_running; }

  void setup_dockspace(ImGuiID dockspace_id);

  void process_input();
  void present();

  void close();

  const char* type_name() { return "ThunderDashboard"; }
  void data_clear();
  bool data_should_open(const char* name);
  void data_read_line(const char* line);
  void data_apply();
  void data_write(const char* type_name, ImGuiTextBuffer* buf);

private:
  void present_menu_bar();
  void present_file_menu();
  void present_tools_menu();

  void startup();
};

