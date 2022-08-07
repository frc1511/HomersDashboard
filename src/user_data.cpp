#include <user_data.h>
#include <pages/settings.h>

#define SECTION_SETTINGS "Settings"
#define KEY_TEAM_NUMBER "TeamNumber"

void UserData::clear_all(ImGuiContext* context, ImGuiSettingsHandler* handler) {
  data_map.clear();
}

void UserData::read_init(ImGuiContext* context, ImGuiSettingsHandler* handler) { }

void* UserData::read_open(ImGuiContext* context, ImGuiSettingsHandler* handler, const char* name) {
  bool res = true;
  
  if (std::strcmp(name, SECTION_SETTINGS) != 0) {
    res = false;
  }
  
  return reinterpret_cast<void*>(res);
}

void UserData::read_line(ImGuiContext* context, ImGuiSettingsHandler* handler, void* entry, const char* line) {
  data_map.insert(line);
}

void UserData::apply_all(ImGuiContext* context, ImGuiSettingsHandler* handler) {
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
    }
  }
}

void UserData::write_all(ImGuiContext* context, ImGuiSettingsHandler* handler, ImGuiTextBuffer* buf) {
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
  }
}