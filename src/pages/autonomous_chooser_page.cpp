#include <HomersDashboard/pages/autonomous_chooser_page.h>

#include <HomersDashboard/imgui_util.h>

#define COLUMN_WIDTH 100

void AutonomousChooserPage::present(bool* running) {
  const std::string auto_list = m_nt_handler.smart_dashboard()->GetString(
      "thunderdashboard_auto_list", "");

  if (auto_list != m_last_auto_list) {
    update_auto_modes(auto_list);
    m_last_auto_list = auto_list;
  }

  if (!ImGui::Begin(name(), running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }

  {
    ImGuiScopedField field("Auto Mode", COLUMN_WIDTH);

    if (ImGui::BeginCombo("##Auto Mode", m_auto_mode_str.c_str())) {
      for (const auto& [mode, name] : m_auto_modes) {
        if (ImGui::Selectable(name.c_str(), m_auto_mode == mode)) {
          m_auto_mode_str = name;
          set_auto_mode(mode);
        }
      }
      ImGui::EndCombo();
    }
  }
  {
    ImGuiScopedField field("Auto Delay", COLUMN_WIDTH);

    if (ImGui::DragFloat("##Auto Delay", &m_auto_delay, 0.1f, 0.0f, 0.0f,
                         "%.2f s")) {
      set_auto_delay(m_auto_delay);
    }
  }

  ImGui::End();
}

void AutonomousChooserPage::set_auto_mode(int mode) {
  m_auto_mode = mode;

  bool contains = m_auto_modes.contains(m_auto_mode);

  if (!contains) {
    m_auto_mode = 0;
    contains = m_auto_modes.contains(m_auto_mode);
  }

  if (contains)
    m_auto_mode_str = m_auto_modes[m_auto_mode];
  else
    m_auto_mode_str = std::to_string(m_auto_mode);

  m_nt_handler.set_double("Auto_Mode", m_auto_mode);
}

void AutonomousChooserPage::set_auto_delay(float delay) {
  m_auto_delay = delay;

  if (m_auto_delay < 0.f) {
    m_auto_delay = 0.f;
  }

  if (m_auto_delay > 15.f) {
    m_auto_delay = 15.f;
  }

  m_nt_handler.set_double("thunderdashboard_auto_start_delay", m_auto_delay);
}

void AutonomousChooserPage::update_auto_modes(const std::string& auto_list) {
  m_auto_modes.clear();

  std::string num;

  std::stringstream ss(auto_list);
  while (ss.good()) {
    getline(ss, num, ',');
    if (num.empty()) continue;

    const std::string desc(m_nt_handler.smart_dashboard()->GetString(
        "thunderdashboard_auto_" + num, ""));

    m_auto_modes[std::atoi(num.c_str())] = num + ": " + desc;
  }

  set_auto_mode(m_auto_mode);
}

