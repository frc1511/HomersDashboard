#include <pages/auto_chooser.h>
#include <imgui_internal.h>

#include <networktables/EntryListenerFlags.h>
#include <networktables/NetworkTableInstance.h>

#include <app.h>

#define COL_WIDTH 100

AutoChooserPage::AutoChooserPage() = default;

AutoChooserPage::~AutoChooserPage() = default;

void AutoChooserPage::init() {
  mode_listener = App::get()->get_nt_sd_table()->AddEntryListener(
    "thunderdashboard_auto_list",
    // Updates the auto modes when the list entry is changed.
    [&](nt::NetworkTable*, std::string_view, nt::NetworkTableEntry, std::shared_ptr<nt::Value> value, int) -> void {
      auto_modes.clear();
      std::string auto_list(value->GetString());
      
      std::stringstream ss(auto_list);
      while (ss.good()) {
        std::string sub;
        getline(ss, sub, ',');
        if (sub == "") continue;
        
        std::string desc(App::get()->get_nt_sd_table()->GetString("thunderdashboard_auto_" + sub, ""));
        auto_modes.insert({ std::atoi(sub.c_str()), sub + ": " + desc });
      }
    },
    nt::EntryListenerFlags::kUpdate | nt::EntryListenerFlags::kNew | nt::EntryListenerFlags::kLocal
  );
}

void AutoChooserPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_FirstUseEver);
  
  if (!ImGui::Begin("Auto Chooser", running, ImGuiWindowFlags_NoCollapse)) {

    ImGui::End();
    return;
  }

  focused = ImGui::IsWindowFocused();
  
  ImGui::PushID("Auto Mode");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("Auto Mode");
  ImGui::NextColumn();
  
  if (ImGui::BeginCombo("##Auto Mode", auto_mode_str.c_str())) {
    for (auto& [num, name] : auto_modes) {
      if (ImGui::Selectable(name.c_str(), auto_mode == num)) {
        App::get()->get_nt_sd_table()->PutNumber("Auto_Mode", auto_mode);
        auto_mode = num;
        auto_mode_str = name;
      }
    }
    ImGui::EndCombo();
  }
  
  ImGui::Columns(1);
  ImGui::PopID();
  
  ImGui::PushID("Auto Delay");
  ImGui::Columns(2, nullptr, false);
  ImGui::SetColumnWidth(0, COL_WIDTH);
  ImGui::Text("Auto Delay");
  ImGui::NextColumn();
  
  if (ImGui::DragFloat("##Auto Delay", &auto_delay, 0.1f, 0.0f, 0.0f, "%.2f s")) {
    if (auto_delay < 0.0f) {
      auto_delay = 0.0f;
    }
    if (auto_delay > 10.0f) {
      auto_delay = 10.0f;
    }
    
    App::get()->get_nt_sd_table()->PutNumber("Auto_Delay", auto_delay);
  }
  
  ImGui::Columns(1);
  ImGui::PopID();
  
  ImGui::End();
}

void AutoChooserPage::set_auto_mode(int mode) {
  auto_mode = mode;
  
  if (auto_mode < 0 || auto_mode >= static_cast<int>(auto_modes.size())) {
    auto_mode = 0;
  }

  App::get()->get_nt_sd_table()->PutNumber("Auto_Mode", auto_mode);
}

void AutoChooserPage::set_auto_delay(double delay) {
  auto_delay = delay;
  
  if (auto_delay < 0.0) {
    auto_delay = 0.0;
  }
  
  if (auto_delay > 10.0) {
    auto_delay = 10.0;
  }

  App::get()->get_nt_sd_table()->PutNumber("Auto_Delay", auto_delay);
}

AutoChooserPage AutoChooserPage::instance;
