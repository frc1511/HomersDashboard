#pragma once

#include <HomersDashboard/homers_dashboard.h>

class Service {
public:
  virtual ~Service() = default;

  virtual void init() {}
  virtual void process() {}

protected:
  Service() = default;
};

