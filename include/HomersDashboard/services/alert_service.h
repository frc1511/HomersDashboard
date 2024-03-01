#pragma once

#include <HomersDashboard/nt_handler.h>
#include <HomersDashboard/services/service.h>

class AlertService : public Service {
  NTHandler& m_nt_handler;

  bool m_blink = false;
  std::chrono::steady_clock::time_point m_blink_clock;

public:
  AlertService(NTHandler& nt_handler)
    : m_nt_handler(nt_handler) {}

  void init() override;
  void process() override;
};

