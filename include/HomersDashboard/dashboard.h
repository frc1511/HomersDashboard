#pragma once

#include <ThunderDashboard/dashboard.h>
#include <ThunderDashboard/pages/page.h>
#include <ThunderDashboard/services/service.h>
#include <unordered_set>

class HomersDashboard : public frc1511::ThunderDashboard {
public:
  static HomersDashboard* get() {
    return &instance;
  }

  HomersDashboard(HomersDashboard const&) = delete;
  HomersDashboard& operator=(HomersDashboard const&) = delete;

  void init() override;
  void present() override;
  void close() override;
  void handle_keyboard(int key, int scancode, int action, int mods) override;

  void data_clear() override;
  bool data_should_open(const char* name) override;
  void data_read_line(const char* line) override;
  void data_apply(const char* type_name) override;
  void data_write(const char* type_name, ImGuiTextBuffer* buffer) override;

  inline const char* get_data_name() override { return "Homer's Dashboard"; }

private:
  HomersDashboard();
  ~HomersDashboard();

  enum class EventState {
    NONE = 0,
    CLOSE,
  };

  bool initialized = false;

  EventState event_state = EventState::NONE;
  
  uint64_t page_states = 0;

  void update_page_states();
  
  std::map<std::string, std::map<std::string, std::string>> data_map;

  const std::vector<frc1511::Page*> all_pages;
  const std::vector<frc1511::Service*> all_services;

  bool app_section = false;
  decltype(all_pages)::const_iterator current_page = all_pages.cend();

  static HomersDashboard instance;
};
