#pragma once

#include <ThunderDashboard/pages/page.h>
#include <ThunderDashboard/nt_handler.h>

class NetworkTablesPage : public frc1511::Page {
public:
  static NetworkTablesPage* get() {
    return &instance;
  }
  
  NetworkTablesPage(NetworkTablesPage const&) = delete;
  NetworkTablesPage& operator=(NetworkTablesPage const&) = delete;
  
  void present(bool* running) override;

private:
  NetworkTablesPage();
  ~NetworkTablesPage();
  
  static NetworkTablesPage instance;
};
