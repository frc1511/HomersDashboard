#include <HomersDashboard/nt_handler.h>

extern "C" {
struct HAL_ControlWord {
  uint32_t enabled : 1;
  uint32_t autonomous : 1;
  uint32_t test : 1;
  uint32_t eStop : 1;
  uint32_t fmsAttached : 1;
  uint32_t dsAttached : 1;
  uint32_t control_reserved : 26;
};
typedef struct HAL_ControlWord HAL_ControlWord;
}

static_assert(sizeof(HAL_ControlWord) == sizeof(int32_t));

void NTHandler::init(Version version, bool ds_running, bool local_server) {
  m_nt_inst = nt::NetworkTableInstance::GetDefault();
  m_sd_table = m_nt_inst.GetTable("SmartDashboard");
  m_fms_table = m_nt_inst.GetTable("FMSInfo");

  const char* identity = "ThunderDashboard";
  if (version == Version::V3) {
    m_nt_inst.StartClient3(identity);
  } else {
    m_nt_inst.StartClient4(identity);
  }

  if (local_server)
    m_nt_inst.SetServer("127.0.0.1");
  else {
    // 10.15.11.2
    m_nt_inst.SetServerTeam(1511);
    if (ds_running) m_nt_inst.StartDSClient();
  }
}

void NTHandler::deinit() { m_nt_inst.StopClient(); }

void NTHandler::update() {
  for (auto& [key, value] : m_set_bools) {
    m_sd_table->PutBoolean(key, value);
  }
  for (auto& [key, value] : m_set_doubles) {
    m_sd_table->PutNumber(key, value);
  }
  for (auto& [key, value] : m_set_strings) {
    m_sd_table->PutString(key, value);
  }
}

NTHandler::MatchMode NTHandler::get_match_mode() {
  const int32_t ctrl_word_int = get_ctrl_word();

  HAL_ControlWord ctrl_word;
  std::memcpy(&ctrl_word, &ctrl_word_int, sizeof(ctrl_word));

  if (!ctrl_word.enabled) {
    return MatchMode::DISABLED;
  } else if (ctrl_word.autonomous) {
    return MatchMode::AUTO;
  } else if (ctrl_word.test) {
    return MatchMode::TEST;
  } else if (ctrl_word.eStop) {
    return MatchMode::ESTOPPED;
  } else {
    return MatchMode::TELEOP;
  }
}

NTHandler::Alliance NTHandler::get_alliance() {
  if (static_cast<bool>(m_fms_table->GetBoolean("IsRedAlliance", true))) {
    return Alliance::RED;
  } else {
    return Alliance::BLUE;
  }
}

NTHandler::MatchType NTHandler::get_match_type() {
  return static_cast<NTHandler::MatchType>(
      m_fms_table->GetNumber("MatchType", 0));
}

std::size_t NTHandler::get_station_number() {
  return static_cast<std::size_t>(m_fms_table->GetNumber("StationNumber", 1.0));
}

std::string NTHandler::get_event_name() {
  return m_fms_table->GetString("EventName", "");
}

std::string NTHandler::get_game_msg() {
  return m_fms_table->GetString("GameSpecificMessage", "");
}

std::size_t NTHandler::get_match_number() {
  return m_fms_table->GetNumber("MatchNumber", 0.0);
}

std::size_t NTHandler::get_replay_number() {
  return m_fms_table->GetNumber("ReplayNumber", 0.0);
}

void NTHandler::set_bool(const std::string& key, bool value) {
  m_set_bools.insert_or_assign(key, value);
}

void NTHandler::set_double(const std::string& key, double value) {
  m_set_doubles.insert_or_assign(key, value);
}

void NTHandler::set_string(const std::string& key, std::string_view value) {
  m_set_strings.insert_or_assign(key, value);
}

int32_t NTHandler::get_ctrl_word() {
  return static_cast<int32_t>(m_fms_table->GetNumber("FMSControlData", 0.0));
}

