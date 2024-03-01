#pragma once

#include <HomersDashboard/nt_handler.h>
#include <HomersDashboard/pages/page.h>

namespace y2022 {

class ShooterPositionPage : public Page {
  NTHandler& m_nt_handler;

public:
  ShooterPositionPage(NTHandler& nt_handler)
    : m_nt_handler(nt_handler) {}

  const char* name() const override { return "2022 Shooter Position"; }
  const char* short_name() const override { return "Shooter Position"; }
  void present(bool* running) override;
};

} // namespace y2022

