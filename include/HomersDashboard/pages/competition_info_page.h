#pragma once

#include <HomersDashboard/font_library.h>
#include <HomersDashboard/nt_handler.h>
#include <HomersDashboard/pages/page.h>

//
// Displays various information from the field management system (FMS).
//
class CompetitionInfoPage : public Page {
  FontLibrary& m_font_lib;
  NTHandler& m_nt_handler;

public:
  CompetitionInfoPage(FontLibrary& font_lib, NTHandler& nt_handler)
    : m_font_lib(font_lib),
      m_nt_handler(nt_handler) {}

  const char* name() const override { return "Competition Info"; }
  void present(bool* running) override;
};

