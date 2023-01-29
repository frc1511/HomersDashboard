#pragma once

#include <ThunderDashboard/pages/page.h>
#include <ThunderDashboard/nt_handler.h>
#include <imgui_internal.h>

namespace y2023 {

class AutoPreviewPage : public frc1511::Page {
public:
  static AutoPreviewPage* get() {
    return &instance;
  }

  AutoPreviewPage(AutoPreviewPage const&) = delete;
  AutoPreviewPage& operator=(AutoPreviewPage const&) = delete;

  void init() override;
  void present(bool* running) override;

private:
  AutoPreviewPage();
  ~AutoPreviewPage();

  void show_auto_preview();

  enum GamePiece {
    CUBE,
    CONE,
    UNKNOWN,
  };

  // Draws a gamepiece on the field (0-3).
  void draw_gamepiece(GamePiece gp, int index, bool blue_side);

  unsigned int field_tex;
  double field_ar;

  ImVec2 to_draw_coord(ImVec2 pt) const;
  ImVec2 adjust_field_coord(ImVec2 pt) const;

  const ImVec2 min { 0.02800000086426735f, 0.018844246864318848f },
               max { 0.9739999771118164f, 0.9825640916824341f };

  ImDrawList* draw_list;
  ImRect bb;
  
  static AutoPreviewPage instance;
};

} // namespace y2023
