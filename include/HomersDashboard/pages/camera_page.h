#pragma once

#include <HomersDashboard/homers_dashboard.h>
#include <HomersDashboard/pages/page.h>
#include <HomersDashboard/texture.h>

#ifdef HD_WITH_CS
#include <cscore.h>
#include <cscore_cv.h>
#include <opencv2/opencv.hpp>
#endif

class CameraPage : public Page {
  std::string m_name, m_url;

  std::thread m_camera_thread;
  std::mutex m_camera_mutex;

#ifdef HD_WITH_CS
  cv::Mat m_frame;
#endif

  std::unique_ptr<Texture> m_no_cam_tex = nullptr;
  std::unique_ptr<Texture> m_frame_tex = nullptr;

  // Aspect ratios.
  float m_no_cam_ar, m_frame_ar;

  bool m_thread_running = false;
  bool m_thread_terminated = false;
  bool m_has_frame = false, m_new_frame = false;

  bool m_init = false;

protected:
  // Media must be MJPG stream (Content-Type "multipart/x-mixed-replace").
  CameraPage() = default;
  virtual ~CameraPage();

  void init(std::string_view name, std::string_view url);

  Texture get_frame_texture();
  float get_frame_aspect_ratio();

  void terminate();

private:
  void thread_start();

  void open_changed(bool open) override;
};
