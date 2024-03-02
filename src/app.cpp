#include <HomersDashboard/app.h>

#include <HomersDashboard/graphics_manager.h>
#include <IconsFontAwesome5.h>

void App::setup_dockspace(ImGuiID dockspace_id) {
  ImGuiViewport* viewport = ImGui::GetMainViewport();

  bool dockspace_created = ImGui::DockBuilderGetNode(dockspace_id) != nullptr;

  ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

  if (!dockspace_created) {
    puts("Creating dockspace layout");

    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

    auto dockspace_id_down = ImGui::DockBuilderSplitNode(
        dockspace_id, ImGuiDir_Down, 0.22f, nullptr, &dockspace_id);

    auto dockspace_id_left = ImGui::DockBuilderSplitNode(
        dockspace_id, ImGuiDir_Left, 0.3875f, nullptr, &dockspace_id);

    auto dockspace_id_hang = ImGui::DockBuilderSplitNode(
        dockspace_id_left, ImGuiDir_Left, 0.2839f, nullptr, &dockspace_id_left);

    auto dockspace_id_gp = ImGui::DockBuilderSplitNode(
        dockspace_id_hang, ImGuiDir_Down, 0.3f, nullptr, &dockspace_id_hang);

    const auto dockspace_id_arm = ImGui::DockBuilderSplitNode(
        dockspace_id_left, ImGuiDir_Down, 0.6971f, nullptr, &dockspace_id_left);

    const auto dockspace_id_shooter = ImGui::DockBuilderSplitNode(
        dockspace_id_left, ImGuiDir_Down, 0.435f, nullptr, &dockspace_id_left);

    const auto dockspace_id_timer = dockspace_id_left;

    const auto dockspace_id_comp_info = ImGui::DockBuilderSplitNode(
        dockspace_id_down, ImGuiDir_Right, 0.4f, nullptr, &dockspace_id_down);

    const auto dockspace_id_auto = dockspace_id_down;

    ImGui::DockBuilderDockWindow(m_network_tables_page.name(), dockspace_id);
    ImGui::DockBuilderDockWindow(m_robot_position_page.name(), dockspace_id);

    ImGui::DockBuilderDockWindow(m_comp_info_page.name(),
                                 dockspace_id_comp_info);
    ImGui::DockBuilderDockWindow(m_match_timer_page.name(), dockspace_id_timer);
    ImGui::DockBuilderDockWindow(m_auto_chooser_page.name(), dockspace_id_auto);
    ImGui::DockBuilderDockWindow(m_2024_hang_page.name(), dockspace_id_hang);
    ImGui::DockBuilderDockWindow(m_2024_arm_page.name(), dockspace_id_arm);
    ImGui::DockBuilderDockWindow(m_2024_shooter_page.name(),
                                 dockspace_id_shooter);
    ImGui::DockBuilderDockWindow(m_2024_gamepiece_page.name(), dockspace_id_gp);

    m_network_tables_page.set_open(true);
    m_robot_position_page.set_open(true);
    m_comp_info_page.set_open(true);
    m_match_timer_page.set_open(true);
    m_auto_chooser_page.set_open(true);
    m_2024_hang_page.set_open(true);
    m_2024_arm_page.set_open(true);
    m_2024_shooter_page.set_open(true);
    m_2024_gamepiece_page.set_open(true);
  }
}

void App::present() {
  present_menu_bar();

  switch (m_event_state) {
    using enum EventState;
  case NONE:
    break;
  case STARTUP:
    startup();
    break;
  case CLOSE:
    m_running = false;
    if (m_init) m_nt_handler.deinit();
    return;
  }

  if (!m_init) return;

  m_nt_handler.update();
  for (Service* service : m_services) {
    service->process();
  }

  const auto present_pages = [](auto pages) {
    for (Page* page : pages) {
      bool is_open = page->is_open();
      if (is_open) page->present(&is_open);
      page->set_open(is_open);
    }
  };

  present_pages(m_general_pages);
  present_pages(m_2022_pages);
  present_pages(m_2023_pages);
  present_pages(m_2024_pages);
}

void App::close() { m_event_state = EventState::CLOSE; }

void App::present_menu_bar() {
  if (ImGui::BeginMenuBar()) {
    present_file_menu();
    present_tools_menu();

    ImGui::EndMenuBar();
  }
}

#ifdef HD_MACOS
#define CTRL_STR       "Cmd+"
#define CTRL_SHIFT_STR "Cmd+Shift+"
#else
#define CTRL_STR       "Ctrl+"
#define CTRL_SHIFT_STR "Ctrl+Shift+"
#endif

void App::present_file_menu() {
  bool item_close = false;

  if (ImGui::BeginMenu("File")) {
    ImGui::MenuItem(ICON_FA_WINDOW_CLOSE "  Close", CTRL_STR "W", &item_close);
    ImGui::EndMenu();
  }

  if (item_close) close();
}

void App::present_tools_menu() {
  const auto present_page_menu_item = [](Page* page, bool& is_open) {
    ImGui::MenuItem(page->short_name(), nullptr, &is_open);
  };

  const auto present_page_menu_items = [&](const std::set<Page*>& pages) {
    for (Page* page : pages) {
      bool is_open = page->is_open();
      present_page_menu_item(page, is_open);
      page->set_open(is_open);
    }
  };

  if (ImGui::BeginMenu("Tools")) {
    present_page_menu_items(m_general_pages);

    if (ImGui::BeginMenu("2022")) {
      present_page_menu_items(m_2022_pages);
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("2023")) {
      present_page_menu_items(m_2023_pages);
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("2024")) {
      present_page_menu_items(m_2024_pages);
      ImGui::EndMenu();
    }
    ImGui::EndMenu();
  }
}

void App::startup() {
  ImGui::OpenPopup(m_startup_popup.name());

  bool showing_popup = true;
  m_startup_popup.present(&showing_popup);

  if (showing_popup) return;

  const StartupSettings result = m_startup_popup.settings();

  printf("NT Version: %s\n",
         result.nt_version == NTHandler::Version::V4 ? "4" : "3");
  printf("DS Running: %s\n", result.ds_running ? "true" : "false");

  m_nt_handler.init(result.nt_version, result.ds_running,
                    result.local_nt_server);

  m_intake_camera_page.init(result.host_camera_server);
  m_limelight_camera_page.init();

  for (Service* service : m_services) {
    service->init();
  }

  m_event_state = EventState::NONE;
  m_init = true;
}

void App::data_clear() {
  m_window_data.clear();
  m_page_data.clear();
}

bool App::data_should_open(const char* name) {
  m_save_section = SaveSection::NONE;
  m_save_page = nullptr;

  if (std::string_view(name) == "Window_State") {
    m_save_section = SaveSection::WINDOW_STATE;
    return true;
  }

  for (Page* page : m_all_pages) {
    if (std::string_view(name) == page->name()) {
      m_save_section = SaveSection::PAGE_DATA;
      m_save_page = page;
      return true;
    }
  }

  return false;
}

void App::data_read_line(const char* _line) {
  std::string_view line(_line);

  const std::size_t delimeter = line.find("=");
  std::string key(line.substr(0, delimeter));
  std::string value(line.substr(delimeter + 1));

  DataLines* data = nullptr;

  if (m_save_section == SaveSection::WINDOW_STATE) {
    data = &m_window_data;
  } else {
    assert(m_save_page);
    data = &m_page_data[m_save_page];

    if (key == "is_open") {
      m_save_page->set_open(std::stoi(value));
      return;
    }
  }

  (*data)[key] = value;
}

void App::data_apply() {
  // Window data.
  for (const auto& [key, value] : m_window_data) {
    int val = std::atoi(value.c_str());

    // Window dimensions.
    const ImVec2 size = GraphicsManager::get().window_size();
    if (key == "Window_Width") {
      if (!val) continue;
      GraphicsManager::get().set_window_size(val, size.y);
    }
    if (key == "Window_Height") {
      if (!val) continue;
      GraphicsManager::get().set_window_size(size.x, val);
    }

    // Window position.
    const ImVec2 pos = GraphicsManager::get().window_pos();
    if (key == "Window_XPos") {
      GraphicsManager::get().set_window_pos(val, pos.y);
    }
    if (key == "Window_YPos") {
      GraphicsManager::get().set_window_pos(pos.x, val);
    }
  }

  // Page data.
  for (const auto& [page, data] : m_page_data) {
    page->apply_save_data(data);
  }
}

void App::data_write(const char* type_name, ImGuiTextBuffer* buf) {
  const auto begin_section = [&](const char* name) {
    buf->appendf("[%s][%s]\n", type_name, name);
  };

  const auto end_section = [&]() { buf->append("\n"); };

  // Window state.
  {
    begin_section("Window_State");

    const ImVec2 size = GraphicsManager::get().window_size();
    buf->appendf("Window_Width=%d\n", int(size.x));
    buf->appendf("Window_Height=%d\n", int(size.y));

    const ImVec2 pos = GraphicsManager::get().window_pos();
    buf->appendf("Window_XPos=%d\n", int(pos.x));
    buf->appendf("Window_YPos=%d\n", int(pos.y));

    end_section();
  }

  // Page data.
  for (Page* page : m_all_pages) {
    DataLines data = page->get_save_data();

    begin_section(page->name());
    buf->appendf("%s=%d\n", "is_open", page->is_open());
    for (auto& [key, value] : data) {
      buf->appendf("%s=%s\n", key.c_str(), value.c_str());
    }
    end_section();
  }
}

#if HD_MACOS
#define CTRL_KEY (io.KeySuper)
#else
#define CTRL_KEY (io.KeyCtrl)
#endif

#define CTRL_DOWN       (CTRL_KEY && !io.KeyAlt && !io.KeyShift)
#define CTRL_SHIFT_DOWN (CTRL_KEY && !io.KeyAlt && io.KeyShift)

#define KEY_DOWN(key) (io.KeysData[key].DownDuration == 0.0f)

void App::process_input() {
  const ImGuiIO& io = ImGui::GetIO();

  if (CTRL_DOWN && KEY_DOWN(ImGuiKey_W)) {
    close();
  }
}

