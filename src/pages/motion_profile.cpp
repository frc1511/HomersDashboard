#include <pages/motion_profile.h>
#include <platform/platform.h>
#include <imgui_internal.h>

extern "C" {
  struct HAL_ControlWord {
    uint32_t enabled : 1;
    uint32_t autonomous : 1;
    uint32_t test : 1;
    uint32_t eStop : 1;
    uint32_t fmsAttached : 1;
    uint32_t dsAttached : 1;
    uint32_t control_reserved : 26;
  };
  typedef struct HAL_ControlWord HAL_ControlWord;
}

#define COL_WIDTH 150

MotionProfilePage::MotionProfilePage()
: sd_table(nt::NetworkTableInstance::GetDefault().GetTable("SmartDashboard")),
  fms_info_table(nt::NetworkTableInstance::GetDefault().GetTable("FMSInfo")) { }

MotionProfilePage::~MotionProfilePage() { }

void MotionProfilePage::present(bool* running) {
  ImGui::ShowDemoWindow();
  ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
  
  if (!ImGui::Begin("Motion Profile", running,
                ImGuiWindowFlags_NoCollapse
              | ImGuiWindowFlags_NoScrollbar
              | ImGuiWindowFlags_NoScrollWithMouse
              | ImGuiWindowFlags_UnsavedDocument * unsaved
              )) {

    ImGui::End();
    return;
  }

  focused = ImGui::IsWindowFocused();

  if (ImGui::BeginTabBar("##Mode", ImGuiTabBarFlags_None)) {
    // --- Recording Tab ---
    if (ImGui::BeginTabItem("Recording")) {
      profile_type = ProfileType::RECORDING;

      if (recording) {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
      }

      ImGui::PushID("Type");
      ImGui::Columns(2, nullptr, false);
      ImGui::SetColumnWidth(0, COL_WIDTH);
      ImGui::Text("Type");
      ImGui::NextColumn();

      ImGui::Combo("##Type", (int*)&recording_type, "Autonomous\0Interval\0");

      ImGui::Columns(1);
      ImGui::PopID();

      if (recording_type == RecordingType::INTERVAL) {
        ImGui::PushID("Interval");
        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, COL_WIDTH);
        ImGui::Text("Interval");
        ImGui::NextColumn();

        if (ImGui::DragFloat("##Interval", &recording_interval, 0.1f, 0.0f, 0.0f, "%.2f s")) {
          if (recording_interval < 0.0f) recording_interval = 0.0f;
        }

        ImGui::Columns(1);
        ImGui::PopID();
      }

      ImGui::PushID("Recording Period");
      ImGui::Columns(2, nullptr, false);
      ImGui::SetColumnWidth(0, COL_WIDTH);
      ImGui::Text("Recording Period");
      ImGui::NextColumn();

      if (ImGui::DragFloat("##Recording Period", &recording_period, 0.1f, 0.0f, 0.0f, "%.2f ms")) {
        if (recording_period < 0.0f) recording_period = 0.0f;
      }

      ImGui::Columns(1);
      ImGui::PopID();

      if (recording) {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
      }

      bool record_disabled = false;
      if (recording_interval == 0.0f || recording_period == 0.0f || recording) {
        record_disabled = true;
      }

      if (record_disabled) {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
      }

      if (ImGui::Button("Record")) {
        recording = true;
        record_start_time = record_end_time = record_last_time = std::chrono::high_resolution_clock::now();
        was_in_auto = false;
        recorded_points.clear();
      }

      if (record_disabled) {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
      }

      ImGui::SameLine();

      if (!recording) {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
      }

      bool was_recording = recording;

      if (ImGui::Button("Stop")) {
        recording = false;
      }

      if (!was_recording) {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
      }

      if (recording) {
        record_end_time = std::chrono::high_resolution_clock::now();
        // Milliseconds since last recording iteration.
        double elapsed = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(record_end_time - record_last_time).count()) / 1e6;
        bool new_entry = elapsed >= recording_period;

        // Seconds since start.
        elapsed = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(record_end_time - record_start_time).count()) / 1e9;

        if (new_entry) {
          if (recording_type == RecordingType::INTERVAL || (recording_type == RecordingType::AUTO && was_in_auto)) {
            double x = sd_table->GetNumber("DriveCSV_x_pos", 0.0);
            double y = sd_table->GetNumber("DriveCSV_y_pos", 0.0);
            double dest_x = sd_table->GetNumber("DriveCSV_dest_x_pos", 0.0);
            double dest_y = sd_table->GetNumber("DriveCSV_dest_y_pos", 0.0);
            double vel_x = sd_table->GetNumber("DriveCSV_x_vel", 0.0);
            double vel_y = sd_table->GetNumber("DriveCSV_y_vel", 0.0);
            double vel_theta = sd_table->GetNumber("DriveCSV_ang_vel", 0.0);
            double ang = sd_table->GetNumber("DriveCSV_ang", 0.0);
            double dest_ang = sd_table->GetNumber("DriveCSV_dest_ang", 0.0);

            recorded_points.push_back(Point{ elapsed, x, y, dest_x, dest_y, vel_x, vel_y, vel_theta, ang, dest_ang });
          }

          record_last_time = std::chrono::high_resolution_clock::now();
        }

        if (recording_type == RecordingType::INTERVAL) {
          if (elapsed > recording_interval) {
            recording = false;
          }
        }
        else {
          int32_t ctrl_word_int = static_cast<int32_t>(fms_info_table->GetNumber("FMSControlData", 0.0));

          HAL_ControlWord ctrl_word;
          std::memcpy(&ctrl_word, &ctrl_word_int, sizeof(ctrl_word));

          if (ctrl_word.enabled && ctrl_word.autonomous) {
            if (!was_in_auto) {
              record_start_time = record_last_time = std::chrono::high_resolution_clock::now();
              was_in_auto = true;
            }
          }
          else if (was_in_auto) {
            was_in_auto = false;
            recording = false;
          }
        }

        ImGui::SameLine();
        if (recording_type == RecordingType::AUTO && !was_in_auto) {
          ImGui::Text("Waiting for autonomous to enable...");
        }
        else {
          ImGui::Text("%0.2f s", elapsed);
        }
      }

      ImGui::EndTabItem();

      points = &recorded_points;
    }
    // --- File Tab ---
    if (recording) {
      ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
      ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }

    if (ImGui::BeginTabItem("File")) {
      profile_type = ProfileType::FILE;

      ImGui::Columns(2, nullptr, false);
      ImGui::SetColumnWidth(0, COL_WIDTH);
      ImGui::Text("File Path");
      ImGui::NextColumn();

      if (ImGui::Button("Load File")) {
        std::string path = Platform::get_current()->open_file_dialog(FileType::FILE, "csv");
        if (!path.empty()) {
          std::ifstream file(path);
          if (!file) {
            file_status = "Could not open file";
          }
          else {
            file_status = path;

            std::string file_str = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

            std::string::const_iterator file_iter = file_str.cbegin();

            while (*file_iter != '\n') ++file_iter;
            ++file_iter;

            auto count = [&]() -> std::size_t {
              std::size_t n = 0;
              while (file_iter != file_str.end() && *file_iter != '\n' && *file_iter != ',' && *file_iter != '}') {
                n++;
                file_iter++;
              }
              return n;
            };

            auto get_num = [&]() -> double {
              std::string::const_iterator start = file_iter;
              std::size_t n = count();
              return std::stod(std::string(start, start + n));
            };

            while (file_iter != file_str.cend()) {
              double time(get_num()); ++file_iter;
              double x(get_num()); ++file_iter;
              double y(get_num()); ++file_iter;
              double dest_x(get_num()); ++file_iter;
              double dest_y(get_num()); ++file_iter;
              double vel_x(get_num()); ++file_iter;
              double vel_y(get_num()); ++file_iter;
              double vel_theta(get_num()); ++file_iter;
              double ang(get_num()); ++file_iter;
              double dest_ang(get_num()); ++file_iter;

              file_points.push_back(Point{ time, x, y, dest_x, dest_y, vel_x, vel_y, vel_theta, ang, dest_ang });
            }
          }
        }
      }

      ImGui::SameLine();
      ImGui::TextUnformatted(file_status.c_str());

      ImGui::Columns(1);

      ImGui::EndTabItem();

      points = &file_points;
    }

    if (recording) {
      ImGui::PopItemFlag();
      ImGui::PopStyleVar();
    }
    ImGui::EndTabBar();
  }

  if (points) {
    ImGui::Separator();
    present_graph();
  }
  
  ImGui::End();
}

void MotionProfilePage::present_graph() {
  const char* graph_types[] = {
    "Time",
    "Position",
  };

  static int graph_type = 0;

  static bool show_x_pos = false,
              show_y_pos = false,
              show_target_x_pos = false,
              show_target_y_pos = false,
              show_x_vel = false,
              show_y_vel = false,
              show_ang_vel = false,
              show_ang = false,
              show_target_ang = false;

  static bool show_pos = false,
              show_target_pos = false;

  if (ImGui::CollapsingHeader("Graph Settings")) {
    ImGui::Combo("Graph Type", &graph_type, graph_types, 2);

    if (graph_type == 0) {
      ImGui::Columns(3, nullptr, false);
      ImGui::SetColumnWidth(0, COL_WIDTH);
      ImGui::SetColumnWidth(1, COL_WIDTH);
      ImGui::Checkbox("X Position", &show_x_pos);
      ImGui::Checkbox("Y Position", &show_y_pos);
      ImGui::Checkbox("Target X Position", &show_target_x_pos);
      ImGui::Checkbox("Target Y Position", &show_target_y_pos);

      ImGui::NextColumn();

      ImGui::Checkbox("X Velocity", &show_x_vel);
      ImGui::Checkbox("Y Velocity", &show_y_vel);
      ImGui::Checkbox("Angular Velocity", &show_ang_vel);

      ImGui::NextColumn();

      ImGui::Checkbox("Angle", &show_ang);
      ImGui::Checkbox("Target Angle", &show_target_ang);

      ImGui::Columns(1);
    }
    else {
      ImGui::Columns(2, nullptr, false);
      ImGui::SetColumnWidth(0, COL_WIDTH);
      ImGui::Checkbox("Recorded Position", &show_pos);
      ImGui::NextColumn();
      ImGui::Checkbox("Target Position", &show_target_pos);
      ImGui::Columns(1);
    }
  }

  ImGui::Separator();

  const ImGuiStyle& style(ImGui::GetStyle());
  const ImGuiIO& io(ImGui::GetIO());
  ImDrawList* draw_list(ImGui::GetWindowDrawList());
  ImGuiWindow* win(ImGui::GetCurrentWindow());
  if (win->SkipItems) return;

  ImGui::SetScrollX(0);
  ImGui::SetScrollY(0);

  // --- Setup the canvas ---

  // Fit the canvas to the window.
  ImVec2 region_avail(ImGui::GetContentRegionAvail());

  float dim_x(region_avail.x),
        dim_y(region_avail.y);

  ImVec2 canvas(dim_x, dim_y);

  ImRect bb = ImRect(win->DC.CursorPos, win->DC.CursorPos + canvas);
  ImRect bg_bb = ImRect(ImVec2(bb.Min.x + 30.0f, bb.Min.y), ImVec2(bb.Max.x, bb.Max.y - 20.0f));

  ImGui::ItemSize(bb);
  if (!ImGui::ItemAdd(bb, 0)) return;

  // --- Draw the canvas ---

  draw_list->AddRectFilled(bg_bb.Min, bg_bb.Max, ImGui::GetColorU32(ImGuiCol_FrameBg));

  // --- Plot the data ---

  double min_time(0.0f), max_time(1.0f),
         min_x, max_x, min_y, max_y,
         min_target_x, max_target_x, min_target_y, max_target_y,
         min_vel_x, max_vel_x, min_vel_y, max_vel_y,
         min_vel_theta, max_vel_theta,
         min_ang, max_ang, min_target_ang, max_target_ang;

  for (auto it(points->cbegin()); it != points->cend(); ++it) {
    auto [time, x, y, target_x, target_y, vel_x, vel_y, vel_theta, ang, target_ang] = *it;
    if (it == points->cbegin()) {
      min_time = max_time = time;
      min_x = max_x = x;
      min_y = max_y = y;
      min_target_x = max_target_x = target_x;
      min_target_y = max_target_y = target_y;
      min_vel_x = max_vel_x = vel_x;
      min_vel_y = max_vel_y = vel_y;
      min_vel_theta = max_vel_theta = vel_theta;
      min_ang = max_ang = ang;
      min_target_ang = max_target_ang = target_ang;
    }
    else {
      min_time = std::min(min_time, time);
      max_time = std::max(max_time, time);
      min_x = std::min(min_x, x);
      max_x = std::max(max_x, x);
      min_y = std::min(min_y, y);
      max_y = std::max(max_y, y);
      min_target_x = std::min(min_target_x, target_x);
      max_target_x = std::max(max_target_x, target_x);
      min_target_y = std::min(min_target_y, target_y);
      max_target_y = std::max(max_target_y, target_y);
      min_vel_x = std::min(min_vel_x, vel_x);
      max_vel_x = std::max(max_vel_x, vel_x);
      min_vel_y = std::min(min_vel_y, vel_y);
      max_vel_y = std::max(max_vel_y, vel_y);
      min_vel_theta = std::min(min_vel_theta, vel_theta);
      max_vel_theta = std::max(max_vel_theta, vel_theta);
      min_ang = std::min(min_ang, ang);
      max_ang = std::max(max_ang, ang);
      min_target_ang = std::min(min_target_ang, target_ang);
      max_target_ang = std::max(max_target_ang, target_ang);
    }
  }

  // --- Draw axis lines ---

  draw_list->AddLine(bg_bb.Min, ImVec2(bg_bb.Min.x, bg_bb.Max.y), ImGui::GetColorU32(ImGuiCol_Border));
  draw_list->AddLine(ImVec2(bg_bb.Min.x, bg_bb.Max.y), bg_bb.Max, ImGui::GetColorU32(ImGuiCol_Border));

  // --- Draw the points ---

  if (graph_type == 0) {
    std::vector<double> max_values;
    std::vector<double> min_values;
    if (show_x_pos) { max_values.push_back(max_x); min_values.push_back(min_x); }
    if (show_y_pos) { max_values.push_back(max_y); min_values.push_back(min_y); }
    if (show_target_x_pos) { max_values.push_back(max_target_x); min_values.push_back(min_target_x); }
    if (show_target_y_pos) { max_values.push_back(max_target_y); min_values.push_back(min_target_y); }
    if (show_x_vel) { max_values.push_back(max_vel_x); min_values.push_back(min_vel_x); }
    if (show_y_vel) { max_values.push_back(max_vel_y); min_values.push_back(min_vel_y); }
    if (show_ang_vel) { max_values.push_back(max_vel_theta); min_values.push_back(min_vel_theta); }
    if (show_ang) { max_values.push_back(max_ang); min_values.push_back(min_ang); }
    if (show_target_ang) { max_values.push_back(max_target_ang); min_values.push_back(min_target_ang); }

    if (max_values.empty()) { max_values.push_back(1.0); }
    if (min_values.empty()) { min_values.push_back(0.0); }

    double max_val = *std::max_element(max_values.begin(), max_values.end()),
          min_val = *std::min_element(min_values.begin(), min_values.end());

    if (max_val == min_val) { max_val += 1.0; }

    // --- Draw axes ---

    auto min_time_str = fmt::format("{:.2f}", min_time),
        max_time_str = fmt::format("{:.2f}", max_time),
        min_val_str = fmt::format("{:.2f}", min_val),
        max_val_str = fmt::format("{:.2f}", max_val);

    draw_list->AddText(ImVec2(bb.Min.x + 30.0f, bb.Max.y - 20.0f), ImGui::GetColorU32(ImGuiCol_Text), min_time_str.c_str());
    draw_list->AddText(ImVec2(bb.Max.x - (6.0f * max_time_str.length()), bb.Max.y - 20.0f), ImGui::GetColorU32(ImGuiCol_Text), max_time_str.c_str());
    draw_list->AddText(ImVec2(bb.Min.x, bb.Max.y - 40.0f), ImGui::GetColorU32(ImGuiCol_Text), min_val_str.c_str());
    draw_list->AddText(ImVec2(bb.Min.x, bb.Min.y), ImGui::GetColorU32(ImGuiCol_Text), max_val_str.c_str());

    // --- Draw line at 0 ---

    if (min_val <= 0.0 && max_val >= 0.0) {
      float y = bg_bb.Max.y - (bg_bb.Max.y - bg_bb.Min.y) * (0.0 - min_val) / (max_val - min_val);
      draw_list->AddLine(ImVec2(bg_bb.Min.x, y), ImVec2(bg_bb.Max.x, y), ImGui::GetColorU32(ImGuiCol_Border));

      draw_list->AddText(ImVec2(bb.Min.x, y - 10.0f), ImGui::GetColorU32(ImGuiCol_Text), "0.00");
    }

    // --- Draw points ---

    for (auto it(points->cbegin()); it != points->cend(); ++it) {
      auto [time, x, y, target_x, target_y, vel_x, vel_y, vel_theta, ang, target_ang] = *it;

      double t_norm((time - min_time) / (max_time - min_time));

      auto plot_time = [&](double val, ImColor col) {
        // Normalize y based on min and max values.
        val = (val - min_val) / (max_val - min_val);

        if (!std::isnan(t_norm) && !std::isnan(val)) {
          draw_list->AddCircle(ImVec2(t_norm, 1 - val) * (bg_bb.Max - bg_bb.Min) + bg_bb.Min, 2, col);
        }
      };

      if (show_x_pos) { plot_time(x, ImColor(1.0f, 0.0f, 0.0f)); }
      if (show_y_pos) { plot_time(y, ImColor(1.0f, 0.64f, 0.0f)); }
      if (show_target_x_pos) { plot_time(target_x, ImColor(1.0f, 1.0f, 0.0f)); }
      if (show_target_y_pos) { plot_time(target_y, ImColor(0.0f, 1.0f, 0.0f)); }
      if (show_x_vel) { plot_time(vel_x, ImColor(0.0f, 0.6f, 0.6f)); }
      if (show_y_vel) { plot_time(vel_y, ImColor(0.0f, 0.0f, 1.0f)); }
      if (show_ang_vel) { plot_time(vel_theta, ImColor(0.9f, 0.9f, 0.98f)); }
      if (show_ang) { plot_time(ang, ImColor(1.0f, 0.0f, 1.0f)); }
      if (show_target_ang) { plot_time(target_ang, ImColor(1.0f, 0.75f, 0.8f)); }
    }
  }
  else if (graph_type == 1) {
    std::vector<double> max_x_values, min_x_values, max_y_values, min_y_values;
    if (show_pos) {
      max_x_values.push_back(max_x); min_x_values.push_back(min_x);
      max_y_values.push_back(max_y); min_y_values.push_back(min_y);
    }
    if (show_target_pos) {
      max_x_values.push_back(max_target_x); min_x_values.push_back(min_target_x);
      max_y_values.push_back(max_target_y); min_y_values.push_back(min_target_y);
    }

    if (max_x_values.empty()) { max_x_values.push_back(1.0); }
    if (min_x_values.empty()) { min_x_values.push_back(0.0); }
    if (max_y_values.empty()) { max_y_values.push_back(1.0); }
    if (min_y_values.empty()) { min_y_values.push_back(0.0); }

    double max_x_val = *std::max_element(max_x_values.begin(), max_x_values.end()),
           min_x_val = *std::min_element(min_x_values.begin(), min_x_values.end()),
           max_y_val = *std::max_element(max_y_values.begin(), max_y_values.end()),
           min_y_val = *std::min_element(min_y_values.begin(), min_y_values.end());

    if (max_x_val == min_x_val) { max_x_val += 1.0; }
    if (max_y_val == min_y_val) { max_y_val += 1.0; }
    
    // --- Draw axes ---

    auto min_x_val_str = fmt::format("{:.2f}", min_x_val),
        max_x_val_str = fmt::format("{:.2f}", max_x_val),
        min_y_val_str = fmt::format("{:.2f}", min_y_val),
        max_y_val_str = fmt::format("{:.2f}", max_y_val);

    draw_list->AddText(ImVec2(bb.Min.x + 30.0f, bb.Max.y - 20.0f), ImGui::GetColorU32(ImGuiCol_Text), min_x_val_str.c_str());
    draw_list->AddText(ImVec2(bb.Max.x - (6.0f * max_x_val_str.length()), bb.Max.y - 20.0f), ImGui::GetColorU32(ImGuiCol_Text), max_x_val_str.c_str());
    draw_list->AddText(ImVec2(bb.Min.x, bb.Max.y - 40.0f), ImGui::GetColorU32(ImGuiCol_Text), min_y_val_str.c_str());
    draw_list->AddText(ImVec2(bb.Min.x, bb.Min.y), ImGui::GetColorU32(ImGuiCol_Text), max_y_val_str.c_str());

    // --- Draw points ---

    for (auto it(points->cbegin()); it != points->cend(); ++it) {
      if (show_pos) {
        double x(it->x), y(it->y);

        double x_norm((x - min_x_val) / (max_x_val - min_x_val)),
               y_norm((y - min_y_val) / (max_y_val - min_y_val));

        if (!std::isnan(x_norm) && !std::isnan(y_norm)) {
          draw_list->AddCircle(ImVec2(x_norm, 1 - y_norm) * (bg_bb.Max - bg_bb.Min) + bg_bb.Min, 2, ImColor(1.0f, 0.0f, 0.0f, 1.0f));
        }
      }
      if (show_target_pos) {
        double dest_x = it->dest_x,
               dest_y = it->dest_y;
        
        double dest_x_norm = (dest_x - min_x_val) / (max_x_val - min_x_val),
               dest_y_norm = (dest_y - min_y_val) / (max_y_val - min_y_val);

        if (!std::isnan(dest_x_norm) && !std::isnan(dest_y_norm)) {
          draw_list->AddCircle(ImVec2(dest_x_norm, 1 - dest_y_norm) * (bg_bb.Max - bg_bb.Min) + bg_bb.Min, 2, ImColor(0.0f, 1.0f, 0.0f, 1.0f));
        }
      }
    }
  }
}

MotionProfilePage MotionProfilePage::instance {};
