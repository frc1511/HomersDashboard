#pragma once

#include <HomersDashboard/homers_dashboard.h>

class Page {
  bool m_open = false;

public:
  virtual const char* short_name() const { return name(); }
  virtual const char* name() const = 0;
  virtual void present(bool* running) = 0;

  using SaveData = std::map<std::string, std::string>;

  virtual void apply_save_data(SaveData data) { (void)data; }
  virtual SaveData get_save_data() const { return {}; }

  bool is_open() const { return m_open; }
  void set_open(bool open) {
    const bool prev_open = m_open;
    m_open = open;

    if (m_open != prev_open) open_changed(open);
  }

protected:
  Page() = default;
  virtual ~Page() = default;

  virtual void open_changed(bool open) { (void)open; };
};

