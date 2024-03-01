#pragma once

#include <HomersDashboard/homers_dashboard.h>

class Popup {
public:
  virtual const char* name() = 0;
  virtual void present(bool* running) = 0;

protected:
  Popup() = default;
  virtual ~Popup() = default;
};

