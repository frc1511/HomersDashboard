#pragma once

#include <ThunderDashboard/pages/page.h>
#include <ThunderDashboard/nt_handler.h>
#include <imgui_internal.h>

class RobotPositionPage : public frc1511::Page {
public:
  static RobotPositionPage* get() {
    return &instance;
  }
  
  RobotPositionPage(RobotPositionPage const&) = delete;
  RobotPositionPage& operator=(RobotPositionPage const&) = delete;

  void init() override;
  
  void present(bool* running) override;
  
private:
  RobotPositionPage();
  ~RobotPositionPage();

  void show_robot_position();

  ImVec2 to_draw_coord(ImVec2 pt) const;
  ImVec2 adjust_field_coord(ImVec2 pt) const;
  
  unsigned int field_tex;
  double field_ar = 1.0;

  ImVec2 min { 0.116f, 0.16f },
         max { 0.885f, 0.84f };

  ImRect bb;
  
  static RobotPositionPage instance;
};
