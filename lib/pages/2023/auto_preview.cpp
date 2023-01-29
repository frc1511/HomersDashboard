#include <HomersDashboard/pages/2023/auto_preview.h>
#include <glad/glad.h>
#include <stb_image.h>
#include <imgui_internal.h>
#include <HomersDashboard/pages/2023/auto_config.h>

#include <_2023_field_auto_png.h>

#define COL_WIDTH 100

using namespace y2023;

// Field dimensions (meters).
#define FIELD_X 16.54175
#define FIELD_Y 8.0137

AutoPreviewPage::AutoPreviewPage() = default;

AutoPreviewPage::~AutoPreviewPage() = default;

void AutoPreviewPage::init() {
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

    field_ar = static_cast<double>(width) / static_cast<double>(height);

    return tex;
  };

  field_tex = gen_tex(_2023_field_auto_png, _2023_field_auto_png_size);
}

void AutoPreviewPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("2023 Auto Preview", running, 
                    ImGuiWindowFlags_NoCollapse
                  | ImGuiWindowFlags_NoScrollbar
                  | ImGuiWindowFlags_NoScrollWithMouse)) {
    ImGui::End();
    return;
  }
  
  focused = ImGui::IsWindowFocused();

  show_auto_preview();

  ImGui::End();
}

void AutoPreviewPage::show_auto_preview() {
  draw_list = ImGui::GetWindowDrawList();
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
  if ((dim_x / dim_y) > field_ar) {
    dim_x = dim_y * field_ar;
  }
  else {
    dim_y = dim_x / field_ar;
  }

  ImVec2 canvas(dim_x, dim_y);

  bb = ImRect(win->DC.CursorPos, win->DC.CursorPos + canvas);

  ImGui::ItemSize(bb);
  if (!ImGui::ItemAdd(bb, 0)) return;

  draw_list->AddImage(reinterpret_cast<void*>(field_tex), bb.Min, bb.Max);

  bool blue_alliance = frc1511::NTHandler::get()->get_alliance() == frc1511::NTHandler::Alliance::BLUE;

  AutoConfigPage::StartingLocation starting_location = AutoConfigPage::get()->get_starting_location();
  AutoConfigPage::GamePiece field_gamepiece = AutoConfigPage::get()->get_field_gamepiece();
  int starting_action = AutoConfigPage::get()->get_starting_action();

  GamePiece game_pieces[4] = { UNKNOWN, UNKNOWN, UNKNOWN, UNKNOWN };

  if (starting_location == AutoConfigPage::BARRIER) {
    game_pieces[0] = (GamePiece)field_gamepiece;
  }
  else if (starting_location == AutoConfigPage::CENTER && starting_action == 1) {
    game_pieces[1] = (GamePiece)field_gamepiece;
  }
  else if (starting_location == AutoConfigPage::EDGE) {
    game_pieces[3] = (GamePiece)field_gamepiece;
  }

  draw_gamepiece(game_pieces[0], 0, blue_alliance);
  draw_gamepiece(game_pieces[1], 1, blue_alliance);
  draw_gamepiece(game_pieces[2], 2, blue_alliance);
  draw_gamepiece(game_pieces[3], 3, blue_alliance);
  draw_gamepiece(GamePiece::UNKNOWN, 0, !blue_alliance);
  draw_gamepiece(GamePiece::UNKNOWN, 1, !blue_alliance);
  draw_gamepiece(GamePiece::UNKNOWN, 2, !blue_alliance);
  draw_gamepiece(GamePiece::UNKNOWN, 3, !blue_alliance);
}

#define GP_DIST 1.2f

#define GP_CENTER ImVec2(FIELD_X / 2.0f - 1.0f, FIELD_Y / 2.0f - 1.3f)

void AutoPreviewPage::draw_gamepiece(GamePiece gp, int index, bool blue_side) {
  index = 3 - index;
  --index;
  if (index <= 0) --index;

  float index_sign = index / std::abs(index);

  ImVec2 coord = GP_CENTER;

  coord.y += index_sign * (GP_DIST / 2.0f);

  if (index != -1 && index != 1) {
    coord.y += index_sign * GP_DIST;
  }

  if (!blue_side) {
    coord.x = FIELD_X - coord.x;
  }

  ImColor col(206, 200, 50, 255);
  if (gp == CUBE) {
    col = ImColor(195, 0, 195, 255);
  }
  else if (gp == UNKNOWN) {
    col = ImColor(83, 83, 83, 255);
  }

  draw_list->AddCircleFilled(to_draw_coord(coord), 8.0f, col);
}

ImVec2 AutoPreviewPage::to_draw_coord(ImVec2 pt) const {
  pt = adjust_field_coord(pt);

  pt.x /= FIELD_X;
  pt.y /= FIELD_Y;
  pt = ImVec2(pt.x, 1 - pt.y) * (bb.Max - bb.Min) + bb.Min;

  return pt;
}

ImVec2 AutoPreviewPage::adjust_field_coord(ImVec2 pt) const {
  pt.x /= FIELD_X;
  pt.y /= FIELD_Y;

  pt.x *= (max.x - min.x);
  pt.y *= (max.y - min.y);

  pt.x += min.x;
  pt.y += min.y;

  pt.x *= FIELD_X;
  pt.y *= FIELD_Y;

  return pt;
}

AutoPreviewPage AutoPreviewPage::instance;
