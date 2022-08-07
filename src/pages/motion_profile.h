#pragma once

#include <pages/page.h>

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
  
  bool unsaved = false;
  
  static MotionProfilePage instance;
};