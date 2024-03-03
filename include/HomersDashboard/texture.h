#pragma once

#include <HomersDashboard/homers_dashboard.h>

class Texture {
#ifdef HD_WINDOWS
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture_view;
  Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
#else
  unsigned int m_texture;
#endif

  int m_width, m_height, m_nr_channels;

public:
  Texture();
  Texture(unsigned char* data, std::size_t size);

  void set_data(unsigned char* data, int width, int height, int nr_channels);

  int width() const { return m_width; }
  int height() const { return m_height; }
  int nr_channels() const { return m_nr_channels; }

  ImTextureID id() const {
    return reinterpret_cast<void*>(
#ifdef HD_WINDOWS
        m_texture_view.Get()
#else
        m_texture
#endif
    );
  }

private:
  void setup();
};
