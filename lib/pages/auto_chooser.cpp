#include <HomersDashboard/pages/auto_chooser.h>
#include <imgui_internal.h>

#define COL_WIDTH 100

AutoChooserPage::AutoChooserPage() = default;

AutoChooserPage::~AutoChooserPage() = default;

void AutoChooserPage::init() {
  mode_sub = frc1511::NTHandler::get()->get_smart_dashboard()->GetStringTopic("thunderdashboard_auto_list").Subscribe("");
  mode_listener = frc1511::NTHandler::get()->get_instance().AddListener(
    mode_sub,
    nt::EventFlags::kValueAll,
    // Updates the auto modes when the list entry is changed.
    [&](const nt::Event& event) {
      std::lock_guard lk(modes_mutex);
      auto_modes.clear();
      std::string auto_list(event.GetValueEventData()->value.GetString());
      
      std::stringstream ss(auto_list);
      while (ss.good()) {
        std::string sub;
        getline(ss, sub, ',');
        if (sub == "") continue;
        
        std::string desc(frc1511::NTHandler::get()->get_smart_dashboard()->GetString("thunderdashboard_auto_" + sub, ""));
        auto_modes.insert({ std::atoi(sub.c_str()), sub + ": " + desc });
      }
    }
  );
}

void AutoChooserPage::apply_save_data(const SaveData& save_data) {
  set_auto_mode(std::atoi(save_data.at("Auto_Mode").c_str()));
  set_auto_delay(std::atof(save_data.at("Auto_Delay").c_str()));
}

frc1511::Page::SaveData AutoChooserPage::get_save_data() const {
  return {
    { "Auto_Mode", std::to_string(auto_mode) },
    { "Auto_Delay", std::to_string(auto_delay) }
  };
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
    std::lock_guard lk(modes_mutex);
    for (auto& [num, name] : auto_modes) {
      if (ImGui::Selectable(name.c_str(), auto_mode == num)) {
        frc1511::NTHandler::get()->set_double("Auto_Mode", auto_mode);
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
    
    set_auto_delay(auto_delay);
  }
  
  ImGui::Columns(1);
  ImGui::PopID();
  
  ImGui::End();
}

void AutoChooserPage::set_auto_mode(int mode) {
  auto_mode = mode;

  int sz;
  {
    std::lock_guard lk(modes_mutex);
    sz = auto_modes.size();
  }
  
  if (auto_mode < 0 || auto_mode >= sz) {
    auto_mode = 0;
  }

  frc1511::NTHandler::get()->set_double("Auto_Mode", auto_mode);
}

void AutoChooserPage::set_auto_delay(double delay) {
  auto_delay = delay;
  
  if (auto_delay < 0.0) {
    auto_delay = 0.0;
  }
  
  if (auto_delay > 10.0) {
    auto_delay = 10.0;
  }

  frc1511::NTHandler::get()->set_double("thunderdashboard_auto_start_delay", auto_delay);
}

AutoChooserPage AutoChooserPage::instance;
