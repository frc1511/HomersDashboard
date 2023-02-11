#pragma once

#include <ThunderDashboard/pages/page.h>
#include <ThunderDashboard/nt_handler.h>

class BlinkyBlinkyPage : public frc1511::Page {
public:
  static BlinkyBlinkyPage* get() {
    return &instance;
  }

  BlinkyBlinkyPage(BlinkyBlinkyPage const&) = delete;
  BlinkyBlinkyPage& operator=(BlinkyBlinkyPage const&) = delete;

  inline const char* get_save_name() const override { return "Blinky_Blinky"; }
  void apply_save_data(const SaveData& save_data) override;
  SaveData get_save_data() const override;
  void present(bool* running) override;

  constexpr int get_led_mode() const { return led_mode; }
  void set_led_mode(int mode);

  inline const std::tuple<float, float, float> get_custom_color() const { return std::make_tuple(custom_color[0], custom_color[1], custom_color[2]); }

  void set_custom_r(float r);
  void set_custom_g(float g);
  void set_custom_b(float b);

private:
  BlinkyBlinkyPage();
  ~BlinkyBlinkyPage();
  
  int led_mode = 0;
  float custom_color[3] = { 0.0f, 0.0f, 1.0f };
  
  static BlinkyBlinkyPage instance;
};
