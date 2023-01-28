#pragma once

#include <ThunderDashboard/pages/page.h>
#include <ThunderDashboard/nt_handler.h>

namespace y2023 {

class AutoConfigPage : public frc1511::Page {
public:
  static AutoConfigPage* get() {
    return &instance;
  }

  AutoConfigPage(AutoConfigPage const&) = delete;
  AutoConfigPage& operator=(AutoConfigPage const&) = delete;

  void init() override;
  void present(bool* running) override;

private:
  AutoConfigPage();
  ~AutoConfigPage();
  
  static AutoConfigPage instance;
};

} // namespace y2023
