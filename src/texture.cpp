#include <HomersDashboard/texture.h>

#include <HomersDashboard/graphics_manager.h>

#ifdef HD_WINDOWS
#else
#include <glad/glad.h>
#endif

#include <stb_image.h>

Texture::Texture() { setup(); }

Texture::Texture(unsigned char* img, std::size_t img_size) {
  unsigned char* img_data = stbi_load_from_memory(img, img_size, &m_width,
                                                  &m_height, &m_nr_channels, 0);

  setup();
  set_data(img_data, m_width, m_height, m_nr_channels);

#ifdef HD_WINDOWS
    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = m_width;
    desc.Height = m_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    ID3D11Texture2D *pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = img_data;
    subResource.SysMemPitch = desc.Width * 4;
    subResource.SysMemSlicePitch = 0;
    GraphicsManager::get().device()->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    GraphicsManager::get().device()->CreateShaderResourceView(pTexture, &srvDesc, &m_texture);
    pTexture->Release();
#endif

  stbi_image_free(img_data);
}

void Texture::setup() {
#ifdef HD_WINDOWS
#else
  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#endif
}

void Texture::set_data(unsigned char* data, int width, int height,
                       int nr_channels) {
#ifdef HD_WINDOWS
#else
  m_width = width;
  m_height = height;
  m_nr_channels = nr_channels;

  const int tex_channels = (m_nr_channels == 3 ? GL_RGB : GL_RGBA);

  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, tex_channels, m_width, m_height, 0,
               tex_channels, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
#endif
}

