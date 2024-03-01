#pragma once

#include <HomersDashboard/nt_handler.h>
#include <HomersDashboard/pages/page.h>

namespace y2022 {

class BallCountPage : public Page {
  NTHandler& m_nt_handler;

  unsigned int m_no_cargo_tex, m_red_cargo_tex, m_blue_cargo_tex;

public:
  BallCountPage(NTHandler& nt_handler);

  const char* name() const override { return "2022 Ball Count"; }
  const char* short_name() const override { return "Ball Count"; }
  void present(bool* running) override;
};

} // namespace y2022

