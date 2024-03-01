#pragma once

#include <HomersDashboard/font_library.h>
#include <HomersDashboard/nt_handler.h>
#include <HomersDashboard/pages/page.h>

class MatchTimerPage : public Page {
  FontLibrary& m_font_lib;
  NTHandler& m_nt_handler;

public:
  MatchTimerPage(FontLibrary& font_lib, NTHandler& nt_handler)
    : m_font_lib(font_lib),
      m_nt_handler(nt_handler) {}

  const char* name() const override { return "Match Timer"; }
  void present(bool* running) override;
};
  
