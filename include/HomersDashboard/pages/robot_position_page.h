#pragma once

#include <HomersDashboard/nt_handler.h>
#include <HomersDashboard/pages/page.h>
#include <HomersDashboard/texture.h>

class RobotPositionPage : public Page {
  NTHandler& m_nt_handler;

  const Texture m_field_tex;
  const float m_field_ar;

  ImRect m_bb;

public:
  RobotPositionPage(NTHandler& nt_handler);

  const char* name() const override { return "Robot Position"; }
  void present(bool* running) override;

private:
  void show_robot_position();

  ImVec2 to_draw_coord(ImVec2 pt) const;
  ImVec2 adjust_field_coord(ImVec2 pt) const;
};
