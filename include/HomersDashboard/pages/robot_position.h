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

  ImVec2 min { 0.02800000086426735, 0.018844246864318848 },
         max { 0.9739999771118164, 0.9825640916824341 };

  ImRect bb;
  
  static RobotPositionPage instance;
};
