#pragma once

#include <HomersDashboard/nt_handler.h>
#include <HomersDashboard/pages/page.h>

namespace y2024 {

class GamePiecePage : public Page {
  NTHandler& m_nt_handler;

public:
  GamePiecePage(NTHandler& nt_handler)
    : m_nt_handler(nt_handler) {}

  const char* name() const override { return "2024 Game Piece"; }
  const char* short_name() const override { return "Game Piece"; }
  void present(bool* running) override;
};

} // namespace y2024

