#pragma once

#include <HomersDashboard/dashboard.h>

#define FIELD_X 16.54175 // m
#define FIELD_Y 8.0137 // m

namespace Utils {

/**
 * @brief Generates an OpenGL texture from memory.
 *
 * @param img The image data.
 * @param img_size The size of the image data.
 * @param width The width of the image.
 * @param height The height of the image.
 * @param nr_channels The number of channels in the image.
 */
unsigned int generate_texture_from_memory(unsigned char* img, std::size_t img_size, int* width, int* height, int* nr_channels);

} // namespace Utils
