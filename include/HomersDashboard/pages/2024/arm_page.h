#pragma once

#include <HomersDashboard/nt_handler.h>
#include <HomersDashboard/pages/page.h>

namespace y2024 {

class ArmPage : public Page {
  NTHandler& m_nt_handler;

  ImDrawList* m_draw_list;
  ImRect m_bb;

  const float m_aspect_ratio = 0.93f / 0.95f;

public:
  ArmPage(NTHandler& nt_handler)
    : m_nt_handler(nt_handler) {}

  const char* name() const override { return "2024 Arm"; }
  const char* short_name() const override { return "Arm"; }
  void present(bool* running) override;

private:
  float scale_value(float value) const {
    return value * (m_bb.Max.x - m_bb.Min.x);
  }

  ImVec2 fix_pt(ImVec2 pt) const {
    // Scaling PERFECTION
    pt.x += 0.05f;
    pt.x *= 0.95f;
    pt.y *= 0.93f;
    return ImVec2(pt.x, 1 - pt.y) * (m_bb.Max - m_bb.Min) + m_bb.Min;
  }

  ImVec2 rotate_pt_origin(ImVec2 origin, ImVec2 pt, float angle_rad);
  void rotate_pts_origin(ImVec2 origin, ImVec2* pts, size_t num_pts, float angle_rad);

  void draw_arm();

  void draw_polygon(const ImVec2* pts, size_t num_pts, ImVec2 offset, ImVec2 origin, float angle_rad, ImColor color);
  void draw_arm_angle(float angle_rad, ImColor color);
};

} // namespace y2024

