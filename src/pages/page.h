#pragma once

#include <dashboard.h>

class Page {
public:
  virtual void init() { }
  virtual void present(bool* running) = 0;
  constexpr bool is_focused() const { return focused; }
  
protected:
  Page() = default;
  ~Page() = default;

  bool focused = false;
};