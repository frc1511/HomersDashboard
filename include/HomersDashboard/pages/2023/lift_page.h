#pragma once

#include <HomersDashboard/nt_handler.h>
#include <HomersDashboard/pages/page.h>

namespace y2023 {

class LiftPage : public Page {
  NTHandler& m_nt_handler;

  ImDrawList* m_draw_list;
  ImRect m_bb;

public:
  LiftPage(NTHandler& nt_handler)
    : m_nt_handler(nt_handler) {}

  const char* name() const override { return "2023 Lift"; }
  const char* short_name() const override { return "Lift"; }
  void present(bool* running) override;

private:
  ImVec2 fix_pt(ImVec2 pt) const;

  static std::array<double, 3> get_stage_lengths(double extension_percent);

  void draw_lift();
  void draw_arm(double angle, std::array<double, 3> stage_lengths,
                ImColor color, double width = 5.f);
};

} // namespace y2023

