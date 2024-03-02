#include <HomersDashboard/texture.h>
#include <glad/glad.h>
#include <stb_image.h>

Texture::Texture() { setup(); }

Texture::Texture(unsigned char* img, std::size_t img_size) {
  unsigned char* img_data = stbi_load_from_memory(img, img_size, &m_width,
                                                  &m_height, &m_nr_channels, 0);

  setup();
  set_data(img_data, m_width, m_height, m_nr_channels);

  stbi_image_free(img_data);
}

void Texture::setup() {
  glGenTextures(1, &m_texture_id);
  glBindTexture(GL_TEXTURE_2D, m_texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::set_data(unsigned char* data, int width, int height,
                       int nr_channels) {
  m_width = width;
  m_height = height;
  m_nr_channels = nr_channels;

  const int tex_channels = (m_nr_channels == 3 ? GL_RGB : GL_RGBA);

  glBindTexture(GL_TEXTURE_2D, m_texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, tex_channels, m_width, m_height, 0,
               tex_channels, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
}

