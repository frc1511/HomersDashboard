#pragma once

#include <HomersDashboard/nt_handler.h>
#include <HomersDashboard/pages/page.h>

#include <networktables/StringTopic.h>

class AutonomousChooserPage : public Page {
  NTHandler& m_nt_handler;

  int m_auto_mode = 0;
  std::string m_auto_mode_str = "0";

  float m_auto_delay = 0.0;

  std::string m_last_auto_list = "";

  std::map<int, std::string> m_auto_modes;

public:
  AutonomousChooserPage(NTHandler& nt_handler)
    : m_nt_handler(nt_handler) {}

  const char* name() const override { return "Auto Chooser"; }
  void present(bool* running) override;

  int auto_mode() const { return m_auto_mode; }
  float auto_delay() const { return m_auto_delay; }
  void set_auto_mode(int mode);
  void set_auto_delay(float delay);

private:
  void update_auto_modes(const std::string& auto_list);
};
