#pragma once

#include <HomersDashboard/nt_handler.h>
#include <HomersDashboard/pages/page.h>

namespace y2024 {

class HangPage : public Page {
  NTHandler& m_nt_handler;

public:
  HangPage(NTHandler& nt_handler)
    : m_nt_handler(nt_handler) {}

  const char* name() const override { return "2024 Hang"; }
  const char* short_name() const override { return "Hang"; }
  void present(bool* running) override;

private:
  void draw_hang_side(const char* id, float percent, const ImVec2& avail);
};

} // namespace y2024

