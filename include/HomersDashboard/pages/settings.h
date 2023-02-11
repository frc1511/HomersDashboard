#pragma once

#include <ThunderDashboard/pages/page.h>
#include <ThunderDashboard/nt_handler.h>

class SettingsPage: public frc1511::Page {
public:
  static SettingsPage* get() {
    return &instance;
  }
  
  SettingsPage(SettingsPage const&) = delete;
  SettingsPage& operator=(SettingsPage const&) = delete;
  
  inline const char* get_save_name() const override { return "Settings"; }
  void apply_save_data(const SaveData& save_data) override;
  SaveData get_save_data() const override;
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
