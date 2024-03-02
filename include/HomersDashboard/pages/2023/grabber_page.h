#pragma once

#include <HomersDashboard/nt_handler.h>
#include <HomersDashboard/pages/page.h>
#include <HomersDashboard/texture.h>

namespace y2023 {

class GrabberPage : public Page {
  NTHandler& m_nt_handler;

  const Texture m_cube_tex, m_cone_tex, m_cube_none_tex, m_cone_none_tex,
      m_gp_none_tex;

public:
  GrabberPage(NTHandler& nt_handler);

  const char* name() const override { return "2023 Grabber"; }
  const char* short_name() const override { return "Grabber"; }
  void present(bool* running) override;
};

} // namespace y2023

