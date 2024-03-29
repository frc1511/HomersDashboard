#pragma once

#include <HomersDashboard/nt_handler.h>
#include <HomersDashboard/pages/page.h>

namespace y2024 {

class ShooterPage : public Page {
  NTHandler& m_nt_handler;

public:
  ShooterPage(NTHandler& nt_handler)
    : m_nt_handler(nt_handler) {}

  const char* name() const override { return "2024 Shooter"; }
  const char* short_name() const override { return "Shooter"; }
  void present(bool* running) override;
};

} // namespace y2024

