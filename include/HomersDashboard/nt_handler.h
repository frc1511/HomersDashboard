#pragma once

#include <HomersDashboard/homers_dashboard.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>

class NTHandler {
  nt::NetworkTableInstance m_nt_inst;
  std::shared_ptr<nt::NetworkTable> m_sd_table, m_fms_table;

  std::map<std::string, bool> m_set_bools;
  std::map<std::string, double> m_set_doubles;
  std::map<std::string, std::string> m_set_strings;

public:
  enum class Version {
    V4 = 0,
    V3 = 1,
  };

  void init(Version version, bool ds_running, bool local_server);
  void deinit();
  void update();

  inline nt::NetworkTableInstance instance() { return m_nt_inst; }

  inline std::shared_ptr<nt::NetworkTable> smart_dashboard() {
    return m_sd_table;
  }
  inline std::shared_ptr<nt::NetworkTable> fms_info() { return m_fms_table; }

  //
  // SmartDashboard stuff.
  //

  //
  // You should set NT values using these functions instead of directly using
  // the instance because these will continuously set the values. It's a good
  // way to make sure nothing unexpectedly changes.
  //
  void set_bool(const std::string& key, bool value);
  void set_double(const std::string& key, double value);
  void set_string(const std::string& key, std::string_view value);

  //
  // FMS stuff.
  //

  enum class MatchMode {
    DISABLED,
    AUTO,
    TELEOP,
    TEST,
    ESTOPPED,
  };

  enum class Alliance {
    BLUE,
    RED,
  };

  enum class MatchType {
    NONE,
    PRACTICE,
    QUAL,
    ELIM,
  };

  MatchMode get_match_mode();
  Alliance get_alliance();
  MatchType get_match_type();

  std::size_t get_station_number();
  std::string get_event_name();
  std::string get_game_msg();
  std::size_t get_match_number();
  std::size_t get_replay_number();

private:
  int32_t get_ctrl_word();
};
