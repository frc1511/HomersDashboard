#pragma once

#include <ThunderDashboard/dashboard.h>
#include <unordered_set>

class HomersDashboard : public frc1511::ThunderDashboard {
public:
  static HomersDashboard* get() {
    return &instance;
  }

  HomersDashboard(HomersDashboard const&) = delete;
  HomersDashboard& operator=(HomersDashboard const&) = delete;

  void init() override;
  void present() override;
  void close() override;
  void handle_keyboard(int key, int scancode, int action, int mods) override;

  void data_clear() override;
  bool data_should_open(const char* name) override;
  void data_read_line(const char* line) override;
  void data_apply(const char* type_name) override;
  void data_write(const char* type_name, ImGuiTextBuffer* buffer) override;

  inline const char* get_data_name() override { return "Homer's Dashboard"; }

private:
  HomersDashboard();
  ~HomersDashboard();

  enum class EventState {
    NONE = 0,
    CLOSE,
  };

  EventState event_state = EventState::NONE;
  
  bool show_network_tables = false,
       show_robot_position = false,
       show_auto_chooser   = false,
       show_motion_profile = false,
       show_intake_camera  = false,
       show_limelight      = false,
       show_blinky_blinky  = false,
       show_settings       = false,
       show_match_timer    = false,
       show_comp_info      = false,
       show_2022_ball_count = false,
       show_2022_shooter_pos = false;

  enum Pages {
    PAGE_NETWORK_TABLES = 1 << 0,
    PAGE_ROBOT_POSITION = 1 << 1,
    PAGE_AUTO_CHOOSER   = 1 << 2,
    PAGE_MOTION_PROFILE = 1 << 3,
    PAGE_INTAKE_CAMERA  = 1 << 4,
    PAGE_LIMELIGHT      = 1 << 5,
    PAGE_BLINKY_BLINKY  = 1 << 6,
    PAGE_SETTINGS       = 1 << 7,
    PAGE_MATCH_TIMER    = 1 << 8,
    PAGE_COMP_INFO      = 1 << 9,
    PAGE_2022_BALL_COUNT = 1 << 10,
    PAGE_2022_SHOOTER_POS = 1 << 11,
  };

  unsigned get_page_states() const;
  void set_page_states(unsigned states);

  std::unordered_set<std::string> data_map;

  static HomersDashboard instance;
};
