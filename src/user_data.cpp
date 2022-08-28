#include <user_data.h>
#include <pages/settings.h>
#include <pages/auto_chooser.h>
#include <pages/blinky_blinky.h>

#define SECTION_SETTINGS "Settings"
#define KEY_TEAM_NUMBER "TeamNumber"

#define SECTION_AUTO_CHOOSER "AutoChooser"
#define KEY_AUTO_MODE "AutoMode"
#define KEY_AUTO_DELAY "AutoDelay"

#define SECTION_BLINKY_BLINKY "BlinkyBlinky"
#define KEY_LED_MODE "LedMode"
#define LED_CUSTOM_R "CustomR"
#define LED_CUSTOM_G "CustomG"
#define LED_CUSTOM_B "CustomB"

void UserData::clear_all(ImGuiContext*, ImGuiSettingsHandler*) {
  data_map.clear();
}

void UserData::read_init(ImGuiContext*, ImGuiSettingsHandler*) { }

void* UserData::read_open(ImGuiContext*, ImGuiSettingsHandler*, const char* name) {
  bool res = true;
  
  if (std::strcmp(name, SECTION_SETTINGS) && std::strcmp(name, SECTION_AUTO_CHOOSER) && std::strcmp(name, SECTION_BLINKY_BLINKY)) {
    res = false;
  }
  
  return reinterpret_cast<void*>(res);
}

void UserData::read_line(ImGuiContext*, ImGuiSettingsHandler*, void*, const char* line) {
  data_map.insert(line);
}

void UserData::apply_all(ImGuiContext*, ImGuiSettingsHandler* handler) {
  std::size_t del;
  std::string key;
  std::string val;
  for (auto& line : data_map) {
    del = line.find("=");
    key = line.substr(0, del);
    val = line.substr(del + 1, line.size());
    
    if (std::strcmp(handler->TypeName, USER_DATA_NAME) == 0) {
      if (key == KEY_TEAM_NUMBER) {
        SettingsPage::get()->set_team_number(std::atol(val.c_str()));
      }
      if (key == KEY_AUTO_MODE) {
        AutoChooserPage::get()->set_auto_mode(std::atoi(val.c_str()));
      }
      if (key == KEY_AUTO_DELAY) {
        AutoChooserPage::get()->set_auto_delay(std::atof(val.c_str()));
      }
      if (key == KEY_LED_MODE) {
        BlinkyBlinkyPage::get()->set_led_mode(std::atoi(val.c_str()));
      }
      if (key == LED_CUSTOM_R) {
        BlinkyBlinkyPage::get()->set_custom_r(std::atof(val.c_str()));
      }
      if (key == LED_CUSTOM_G) {
        BlinkyBlinkyPage::get()->set_custom_g(std::atof(val.c_str()));
      }
      if (key == LED_CUSTOM_B) {
        BlinkyBlinkyPage::get()->set_custom_b(std::atof(val.c_str()));
      }
    }
  }
}

void UserData::write_all(ImGuiContext*, ImGuiSettingsHandler* handler, ImGuiTextBuffer* buf) {
  auto begin_section = [&](const char* name) {
    buf->appendf("[%s][%s]\n", handler->TypeName, name);
  };
  
  auto end_section = [&]() {
    buf->append("\n");
  };
  
  auto add_entry = [&](const char* key, const char* val_format, auto ...args) {
    std::string format(std::string("%s=") + val_format + '\n');
    buf->appendf(format.c_str(), key, args...);
  };
  
  if (std::strcmp(handler->TypeName, USER_DATA_NAME) == 0) {
    begin_section(SECTION_SETTINGS);
    add_entry(KEY_TEAM_NUMBER, "%lu", SettingsPage::get()->get_team_number());
    end_section();

    begin_section(SECTION_AUTO_CHOOSER);
    add_entry(KEY_AUTO_MODE, "%d", AutoChooserPage::get()->get_auto_mode());
    add_entry(KEY_AUTO_DELAY, "%f", AutoChooserPage::get()->get_auto_delay());
    end_section();

    begin_section(SECTION_BLINKY_BLINKY);
    add_entry(KEY_LED_MODE, "%d", BlinkyBlinkyPage::get()->get_led_mode());
    auto color(BlinkyBlinkyPage::get()->get_custom_color());
    add_entry(LED_CUSTOM_R, "%f", std::get<0>(color));
    add_entry(LED_CUSTOM_G, "%f", std::get<1>(color));
    add_entry(LED_CUSTOM_B, "%f", std::get<2>(color));
    end_section();
  }
}
