#pragma once

#include <pages/page.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <networktables/TableEntryListener.h>

class AutoChooserPage: public Page {
public:
  static AutoChooserPage* get() {
    return &instance;
  }
  
  AutoChooserPage(AutoChooserPage const&) = delete;
  AutoChooserPage& operator=(AutoChooserPage const&) = delete;

  void init() override;
  
  void present(bool* running) override;

  constexpr int get_auto_mode() const { return auto_mode; }
  void set_auto_mode(int mode);

  constexpr double get_auto_delay() const { return auto_delay; }
  void set_auto_delay(double delay);
  
private:
  AutoChooserPage();
  ~AutoChooserPage();
  
  int auto_mode = 0;
  std::string auto_mode_str = "";
  
  float auto_delay = 0.0;
  
  std::map<int, std::string> auto_modes;
  
  NT_EntryListener mode_listener;
  
  static AutoChooserPage instance;
};
