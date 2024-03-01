#pragma once

#include <HomersDashboard/homers_dashboard.h>

//
// Generates an OpenGL texture from memory.
//
// width, height, and nr_channels are out parameters.
//
unsigned int generate_texture_from_memory(unsigned char* img,
                                          std::size_t img_size, int* width,
                                          int* height, int* nr_channels);

