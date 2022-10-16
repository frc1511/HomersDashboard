#pragma once

#include <ThunderDashboard/pages/page.h>
#include <ThunderDashboard/nt_handler.h>

class CompInfoPage : public frc1511::Page {
public:
  static CompInfoPage* get() {
    return &instance;
  }

  CompInfoPage(CompInfoPage const&) = delete;
  CompInfoPage& operator=(CompInfoPage const&) = delete;

  void present(bool* running) override;

private:
  CompInfoPage();
  ~CompInfoPage();

  static CompInfoPage instance;
};
