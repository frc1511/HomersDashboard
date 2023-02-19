#include <HomersDashboard/utils.h>
#include <glad/glad.h>
#include <stb_image.h>

unsigned int Utils::generate_texture_from_memory(unsigned char* img, std::size_t img_size, int* width, int* height, int* nr_channels) {
  assert(width && height && nr_channels); // Invalid arguments.

  unsigned char* img_data = stbi_load_from_memory(img, img_size, width, height, nr_channels, 0);

  assert(img_data); // Failed to load texture from memory.

  int tex_channels(*nr_channels == 3 ? GL_RGB : GL_RGBA);

  unsigned int tex;

  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, tex_channels, *width, *height, 0, tex_channels, GL_UNSIGNED_BYTE, img_data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(img_data);

  return tex;
}
