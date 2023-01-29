#include <HomersDashboard/pages/2023/node_selector.h>
#include <glad/glad.h>
#include <stb_image.h>
#include <imgui_internal.h>
#include <ThunderDashboard/font_manager.h>

#include <_2023_grid_blue_png.h>
#include <_2023_grid_red_png.h>

#define COL_WIDTH 100

using namespace y2023;

NodeSelectorPage::NodeSelectorPage() = default;

NodeSelectorPage::~NodeSelectorPage() = default;

const std::map<int, float> col_coords = {
  { 0, 0.08 },
  { 1, 0.08 + 0.0935 },
  { 2, 0.08 + 0.0935 * 2 + 0.002 },
  { 3, 0.08 + 0.0935 * 3 + 0.003 },
  { 4, 0.08 + 0.0935 * 4 + 0.005 },
  { 5, 0.08 + 0.0935 * 5 + 0.005 },
  { 6, 0.08 + 0.0935 * 6 + 0.007 },
  { 7, 0.08 + 0.0935 * 7 + 0.007 },
  { 8, 0.08 + 0.0935 * 8 + 0.009 },
};

const std::map<int, float> cone_row_coords = {
  { 0, 0.08 },
  { 1, 0.26 },
  { 2, 0.50 },
};

const std::map<int, float> cube_row_coords = {
  { 0, 0.10 },
  { 1, 0.27 },
  { 2, 0.52 },
};

void NodeSelectorPage::init() {
  auto gen_tex = [&](unsigned char* img, std::size_t img_size) -> unsigned int {
    int width, height, nr_channels;
    unsigned char* img_data = stbi_load_from_memory(img, img_size, &width, &height, &nr_channels, 0);

    assert(img_data); // Failed to load texture from memory.

    int tex_channels(nr_channels == 3 ? GL_RGB : GL_RGBA);

    unsigned int tex;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, tex_channels, width, height, 0, tex_channels, GL_UNSIGNED_BYTE, img_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(img_data);

    grid_ar = static_cast<double>(width) / static_cast<double>(height);

    return tex;
  };

  red_grid_tex = gen_tex(_2023_grid_red_png, _2023_grid_red_png_size);
  blue_grid_tex = gen_tex(_2023_grid_blue_png, _2023_grid_blue_png_size);
}

void NodeSelectorPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("2023 Node Selector", running, 
                    ImGuiWindowFlags_NoCollapse
                  | ImGuiWindowFlags_NoScrollbar
                  | ImGuiWindowFlags_NoScrollWithMouse)) {
    ImGui::End();
    return;
  }
  
  focused = ImGui::IsWindowFocused();

  show_node_selector();

  ImGui::End();
}

void NodeSelectorPage::show_node_selector() {
  ImDrawList* draw_list(ImGui::GetWindowDrawList());
  ImGuiWindow* win(ImGui::GetCurrentWindow());
  if (win->SkipItems) return;

  ImGui::SetScrollX(0);
  ImGui::SetScrollY(0);

  // --- Setup the canvas ---

  // Fit the canvas to the window.
  ImVec2 win_size(ImGui::GetContentRegionAvail());

  float dim_x(win_size.x),
        dim_y(win_size.y);

  // Fit within the window size while maintaining aspect ratio.
  if ((dim_x / dim_y) > grid_ar) {
    dim_x = dim_y * grid_ar;
  }
  else {
    dim_y = dim_x / grid_ar;
  }

  ImVec2 canvas(dim_x, dim_y);

  ImRect bb = ImRect(win->DC.CursorPos, win->DC.CursorPos + canvas);

  ImGui::ItemSize(bb);
  if (!ImGui::ItemAdd(bb, 0)) return;

  frc1511::NTHandler::Alliance alliance = frc1511::NTHandler::get()->get_alliance();
  unsigned int tex = alliance == frc1511::NTHandler::Alliance::BLUE ? blue_grid_tex : red_grid_tex;

  draw_list->AddImage(reinterpret_cast<void*>(tex), bb.Min, bb.Max);

  int group = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_score_grid_group", 1.0);
  int column = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_score_grid_column", 1.0);
  int row = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_score_grid_row", 0.0);

  group = group > 2 ? 2 : group < 0 ? 0 : group;
  column = column > 2 ? 2 : column < 0 ? 0 : column;
  column += group * 3;
  row = row > 2 ? 2 : row < 0 ? 0 : row;

  draw_list->AddRect(bb.Min + ImVec2(col_coords.at(group * 3), 0.0f) * canvas, bb.Min + ImVec2(col_coords.at(group * 3 + 2) + 0.084f, 1.0f) * canvas, ImColor(255, 255, 255, 255), 10.0f, 0, 5.0f);
  draw_list->AddRect(bb.Min + ImVec2(col_coords.at(column), 0.0f) * canvas, bb.Min + ImVec2(col_coords.at(column) + 0.084f, 1.0f) * canvas, ImColor(252, 186, 3, 255), 10.0f, 0, 5.0f);
  draw_list->AddRect(bb.Min + ImVec2(col_coords.at(column), cone_row_coords.at(row)) * canvas, bb.Min + ImVec2(col_coords.at(column) + 0.084f, cone_row_coords.at(row) + 0.23f) * canvas, ImColor(0, 255, 0, 255), 10.0f, 0, 5.0f);
}

NodeSelectorPage NodeSelectorPage::instance;
