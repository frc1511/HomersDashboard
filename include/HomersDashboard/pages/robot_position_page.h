#pragma once

#include <HomersDashboard/nt_handler.h>
#include <HomersDashboard/pages/page.h>

class RobotPositionPage : public Page {
  NTHandler& m_nt_handler;

  unsigned int m_field_tex;
  double m_field_ar = 1.0;

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

