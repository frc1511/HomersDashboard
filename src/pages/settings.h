#pragma once

#include <pages/page.h>

class SettingsPage: public Page {
public:
  static SettingsPage* get() {
    return &instance;
  }
  
  SettingsPage(SettingsPage const&) = delete;
  SettingsPage& operator=(SettingsPage const&) = delete;
  
  void present(bool* running) override;
  
  constexpr std::size_t get_team_number() const { return team_number; }
  void set_team_number(std::size_t number);
  
private:
  SettingsPage();
  ~SettingsPage();
  
  std::size_t team_number = 0;
  char number_buf[10] = "0";

  static SettingsPage instance;
};
