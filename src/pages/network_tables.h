#pragma once

#include <pages/page.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <networktables/TableEntryListener.h>

class NetworkTablesPage : public Page {
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
  
  std::shared_ptr<nt::NetworkTable> sd_table;
  
  static NetworkTablesPage instance;
};
