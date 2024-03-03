#pragma once

#include <HomersDashboard/nt_handler.h>
#include <HomersDashboard/pages/page.h>
#include <HomersDashboard/texture.h>

namespace y2023 {

class ControllerPage : public Page {
  const Texture m_driver_tex, m_aux_tex, m_aux_manual_tex;
  const float m_aspect_ratio;

public:
  ControllerPage();

  const char* name() const override { return "2023 Controller Reference"; }
  const char* short_name() const override { return "Controller Reference"; }
  void present(bool* running) override;
};

} // namespace y2023

