#pragma once

#include <dashboard.h>
#include <imgui_internal.h>
#include <unordered_set>

#define USER_DATA_NAME "Homer's Dashboard"

class UserData {
public:
  static void clear_all(ImGuiContext* context, ImGuiSettingsHandler* handler);
  static void read_init(ImGuiContext* context, ImGuiSettingsHandler* handler);
  static void* read_open(ImGuiContext* context, ImGuiSettingsHandler* handler, const char* name);
  static void read_line(ImGuiContext* context, ImGuiSettingsHandler* handler, void* entry, const char* line);
  static void apply_all(ImGuiContext* context, ImGuiSettingsHandler* handler);
  static void write_all(ImGuiContext* context, ImGuiSettingsHandler* handler, ImGuiTextBuffer* buf);
  
private:
  static inline std::unordered_set<std::string> data_map;
};