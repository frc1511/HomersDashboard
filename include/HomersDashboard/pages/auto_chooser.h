#pragma once

#include <ThunderDashboard/pages/page.h>
#include <ThunderDashboard/nt_handler.h>
#include <networktables/StringTopic.h>

class AutoChooserPage: public frc1511::Page {
public:
  static AutoChooserPage* get() {
    return &instance;
  }
  
  AutoChooserPage(AutoChooserPage const&) = delete;
  AutoChooserPage& operator=(AutoChooserPage const&) = delete;

  void init() override;
  inline const char* get_save_name() const override { return "Auto_Chooser"; }
  void apply_save_data(const SaveData& save_data) override;
  SaveData get_save_data() const override;
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
  
  std::map<int, std::string> blue_auto_modes {
    { 0,  "Do Nothing"     },
    { 1,  "Drive Forwards" },
    { 2,  "Score"          },
    { 3,  "Left: 2GP"      },
    { 4,  "Left: 2GP+CS"   },
    { 5,  "Center: 1GP+CS" },
    { 6,  "Right: 2GP"     },
  };
  
  std::map<int, std::string> red_auto_modes { 
    { 0,  "Do Nothing"     },
    { 1,  "Drive Forwards" },
    { 2,  "Score"          },
    { 3,  "Right: 2GP"      },
    { 4,  "Right: 2GP+CS"   },
    { 5,  "Center: 1GP+CS" },
    { 6,  "Left: 2GP"     },
  };

  std::mutex modes_mutex;
  // nt::StringSubscriber mode_sub;
  // NT_Listener mode_listener;
  
  static AutoChooserPage instance;
};
