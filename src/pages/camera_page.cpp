#include <HomersDashboard/pages/camera_page.h>

#include <no_camera_png.h>

#define CAMERA_FPS   30
#define CAMERA_RES_X 320
#define CAMERA_RES_Y 240

void CameraPage::init(std::string_view name, std::string_view url) {
  assert(!m_init);

  m_name = name;
  m_url = url;
  m_no_cam_tex = std::make_unique<Texture>(no_camera_png, no_camera_png_size);
  m_frame_tex = std::make_unique<Texture>();

  m_no_cam_ar =
      static_cast<float>(m_no_cam_tex->width()) / m_no_cam_tex->height();

  m_camera_thread = std::thread([&]() { thread_start(); });

  m_init = true;
}

CameraPage::~CameraPage() {
  if (!m_init) return;

  terminate();
  m_camera_thread.join();
}

Texture CameraPage::get_frame_texture() {
  std::lock_guard lk(m_camera_mutex);

  if (m_new_frame) {
#ifdef HD_WITH_CS
    m_frame_tex->set_data(m_frame.data, m_frame.cols, m_frame.rows,
#ifdef HD_WINDOWS
                          4
#else
                          3
#endif
    );
#endif
  }

  return m_has_frame ? *m_frame_tex : *m_no_cam_tex;
}

float CameraPage::get_frame_aspect_ratio() {
  std::lock_guard lk(m_camera_mutex);

  return m_has_frame ? m_frame_ar : m_no_cam_ar;
}

void CameraPage::terminate() {
  std::lock_guard lk(m_camera_mutex);
  m_thread_terminated = true;
}

void CameraPage::open_changed(bool open) {
  std::lock_guard lk(m_camera_mutex);
  m_thread_running = open;
}

void CameraPage::thread_start() {
#ifdef HD_WITH_CS
  using namespace std::chrono_literals;

  cs::HttpCamera camera(m_name, m_url,
                        cs::HttpCamera::HttpCameraKind::kMJPGStreamer);
  camera.SetVideoMode(cs::VideoMode::kMJPEG, CAMERA_RES_X, CAMERA_RES_Y,
                      CAMERA_FPS);

  cs::CvSink cv_sink(m_name);
  cv_sink.SetSource(camera);
  cv_sink.SetEnabled(true);

  cv::Mat working_frame;

  std::chrono::steady_clock::time_point start, end;

  while (true) {
    start = std::chrono::high_resolution_clock::now();

    bool running = false;
    {
      std::lock_guard lk(m_camera_mutex);
      if (m_thread_terminated) break;
      running = m_thread_running;
    }

    if (!running) {
      // Try again in 1 second.
      std::this_thread::sleep_for(500ms);
      continue;
    }

    uint64_t frame_time =
        cv_sink.GrabFrame(working_frame); // Returns 0 on fail.

    if (frame_time) {
      // GrabFrame() apparently returns mat in BGR, so convert it to RGB?
      cv::cvtColor(working_frame, working_frame,
#ifdef HD_WINDOWS
                   cv::COLOR_BGR2RGBA
#else
                   cv::COLOR_BGR2RGB
#endif
      );

      // Swap the frame textures.
      {
        std::lock_guard lk(m_camera_mutex);

        // Swap the frame.
        m_frame = working_frame;

        // Aspect ratio.
        m_frame_ar =
            static_cast<float>(m_frame.cols) / static_cast<float>(m_frame.rows);

        m_new_frame = true;
        m_has_frame = true;
      }
    } else {
      std::cout << "Error grabbing frame: " << cv_sink.GetError() << '\n';

      {
        std::lock_guard lk(m_camera_mutex);
        m_new_frame = false;
      }
    }

    end = std::chrono::high_resolution_clock::now();

    const std::chrono::milliseconds dur =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    const std::chrono::milliseconds period = 1000ms / CAMERA_FPS;

    if (dur < period) {
      std::this_thread::sleep_for(period - dur);
    }
  }
#endif
}
