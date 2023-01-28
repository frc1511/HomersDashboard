#pragma once

#include <ThunderDashboard/pages/page.h>
#include <ThunderDashboard/nt_handler.h>

namespace y2023 {

class NodeSelectorPage : public frc1511::Page {
public:
  static NodeSelectorPage* get() {
    return &instance;
  }

  NodeSelectorPage(NodeSelectorPage const&) = delete;
  NodeSelectorPage& operator=(NodeSelectorPage const&) = delete;

  void init() override;
  void present(bool* running) override;

private:
  NodeSelectorPage();
  ~NodeSelectorPage();

  void show_node_selector();

  unsigned int red_grid_tex, blue_grid_tex;

  double grid_ar = 1;
  
  static NodeSelectorPage instance;
};

} // namespace y2023
