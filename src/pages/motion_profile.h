#pragma once

#include <pages/page.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <networktables/TableEntryListener.h>

class MotionProfilePage: public Page {
public:
  static MotionProfilePage* get() {
    return &instance;
  }
  
  MotionProfilePage(MotionProfilePage const&) = delete;
  MotionProfilePage& operator=(MotionProfilePage const&) = delete;
  
  void present(bool* running) override;
  
private:
  MotionProfilePage();
  ~MotionProfilePage();

  void present_graph();

  enum class ProfileType {
    RECORDING = 0,
    FILE = 1,
  };

  ProfileType profile_type = ProfileType::RECORDING;

  enum class RecordingType {
    AUTO = 0,
    INTERVAL = 1,
  };

  RecordingType recording_type = RecordingType::AUTO;

  float recording_interval = 15.0f,
        recording_period = 50.0f;

  bool recording = false;

  std::chrono::high_resolution_clock::time_point record_start_time,
                                                 record_end_time,
                                                 record_last_time;

  bool was_in_auto = false;

  struct Point {
    double time;
    double x;
    double y;
    double t_x;
    double t_y;
    double vel_x;
    double vel_y;
    double vel_theta;
    double ang;
    double t_ang;
  };

  std::vector<Point> recorded_points;
  std::vector<Point> file_points;

  std::string file_status = "No file loaded";

  std::vector<Point>* points = nullptr;
  
  bool unsaved = false;

  std::shared_ptr<nt::NetworkTable> sd_table, fms_info_table;
  
  static MotionProfilePage instance;
};
