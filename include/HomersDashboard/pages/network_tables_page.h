#pragma once

#include <HomersDashboard/font_library.h>
#include <HomersDashboard/nt_handler.h>
#include <HomersDashboard/pages/page.h>

class NetworkTablesPage : public Page {
  FontLibrary& m_font_lib;
  NTHandler& m_nt_handler;

  bool m_init = false;
  std::shared_ptr<nt::NetworkTable> m_root_table;

public:
  NetworkTablesPage(FontLibrary& font_lib, NTHandler& nt_handler)
    : m_font_lib(font_lib),
      m_nt_handler(nt_handler) {}

  const char* name() const override { return "Network Tables"; }
  void present(bool* running) override;

private:
  void show_table(std::string_view path,
                  std::shared_ptr<nt::NetworkTable> table);

  static void show_value(const std::string& key, const nt::Value& value);
  static void show_value_array(const std::string& key, const nt::Value& value);
};
