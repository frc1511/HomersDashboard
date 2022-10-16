#include <HomersDashboard/pages/2022/ball_count.h>
#include <glad/glad.h>
#include <stb_image.h>

#include <cargo_blue_png.h>
#include <cargo_red_png.h>
#include <cargo_none_png.h>

#define COL_WIDTH 100

BallCountPage::BallCountPage() = default;

BallCountPage::~BallCountPage() = default;

void BallCountPage::init() {
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

    return tex;
  };

  no_cargo_tex = gen_tex(cargo_none_png, cargo_none_png_size);
  red_cargo_tex = gen_tex(cargo_red_png, cargo_red_png_size);
  blue_cargo_tex = gen_tex(cargo_blue_png, cargo_blue_png_size);
}

void BallCountPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("Ball Count", running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }
  
  focused = ImGui::IsWindowFocused();

  double ball_count = frc1511::NTHandler::get()->get_smart_dashboard()->GetNumber("thunderdashboard_ballcount", 0.0);
  frc1511::NTHandler::Alliance alliance = frc1511::NTHandler::get()->get_alliance();

  unsigned int tex0 = no_cargo_tex, tex1 = no_cargo_tex;

  auto get_ball_tex = [&]() -> unsigned int {
    return alliance == decltype(alliance)::BLUE ? blue_cargo_tex : red_cargo_tex;
  };

  if (ball_count >= 1) {
    tex0 = get_ball_tex();
  }
  if (ball_count >= 2) {
    tex1 = get_ball_tex();
  }
  
  ImVec2 avail(ImGui::GetContentRegionAvail());
  // Square image.
  float tex_dim = avail.x / 2.0f;

  ImGui::Image(reinterpret_cast<void*>(tex0), ImVec2(tex_dim, tex_dim));
  ImGui::SameLine();
  ImGui::Image(reinterpret_cast<void*>(tex1), ImVec2(tex_dim, tex_dim));

  ImGui::Text("Ball Count: %d", static_cast<int>(ball_count));

  ImGui::End();
}

BallCountPage BallCountPage::instance;
