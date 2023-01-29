#pragma once

#include <ThunderDashboard/pages/page.h>
#include <ThunderDashboard/nt_handler.h>

namespace y2023 {

class AutoConfigPage : public frc1511::Page {
public:
  static AutoConfigPage* get() {
    return &instance;
  }

  AutoConfigPage(AutoConfigPage const&) = delete;
  AutoConfigPage& operator=(AutoConfigPage const&) = delete;

  void init() override;
  void present(bool* running) override;

  enum StartingLocation {
    BARRIER,
    CENTER,
    EDGE,
  };

  enum GamePiece {
    CUBE,
    CONE,
  };

  void set_doing_auto(bool doing);
  void set_starting_location(StartingLocation location);
  void set_starting_gamepiece(GamePiece gp);
  void set_starting_action(int action);
  void set_field_gamepiece(GamePiece gp);
  void set_final_action(int action);

  constexpr bool get_doing_auto() const { return doing_auto; }
  constexpr StartingLocation get_starting_location() const { return starting_location; }
  constexpr GamePiece get_starting_gamepiece() const { return starting_gamepiece; }
  constexpr int get_starting_action() const { return starting_action; }
  constexpr GamePiece get_field_gamepiece() const { return field_gamepiece; }
  constexpr int get_final_action() const { return final_action; }

private:
  AutoConfigPage();
  ~AutoConfigPage();

  void update_nt();

  bool doing_auto = true;
  StartingLocation starting_location = BARRIER;
  GamePiece starting_gamepiece = CUBE;
  int starting_action = 0;
  GamePiece field_gamepiece = CUBE;
  int final_action = 0;
  
  static AutoConfigPage instance;
};

} // namespace y2023
