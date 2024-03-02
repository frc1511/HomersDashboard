#pragma once

#include <HomersDashboard/homers_dashboard.h>

class Texture {
  unsigned int m_texture_id;

  int m_width, m_height, m_nr_channels;

public:
  Texture();
  Texture(unsigned char* data, std::size_t size);

  void set_data(unsigned char* data, int width, int height, int nr_channels);

  int width() const { return m_width; }
  int height() const { return m_height; }
  int nr_channels() const { return m_nr_channels; }

  ImTextureID id() const { return reinterpret_cast<void*>(m_texture_id); }

private:
  void setup();
};

