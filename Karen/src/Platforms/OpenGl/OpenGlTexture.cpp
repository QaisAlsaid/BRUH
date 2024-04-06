#include "pch.h"
#include "Platforms/OpenGl/OpenGlTexture.h"
#include "Platforms/OpenGl/OpenGlCore.h"


namespace Karen
{
  OpenGlTexture2D::OpenGlTexture2D(const std::string& file_path, bool flip_v)
  {
    glCall(glGenTextures(1, &m_renderer_id));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_renderer_id);

    bool status = m_image.loadFromFile(file_path, flip_v);
    KAREN_CORE_ASSERT(status, std::string("Failed to load Texture from file: ") + file_path);
    uint32_t fmt = 0;
    switch(m_image.getColorChannels())
    {
      case 3:
        fmt = GL_RGB;
      case 4:
        fmt = GL_RGBA;
    }
    KAREN_CORE_ASSERT(m_image.getColorChannels() > 2 && m_image.getColorChannels() < 5, "Unsupported Image format");
    KAREN_CORE_TRACE("image: {0} fmt:{1}", file_path, m_image.getColorChannels());
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)); 
    glCall(glTexImage2D(GL_TEXTURE_2D, 0, fmt, m_image.getWidth(), m_image.getHeight()
        , 0, fmt, GL_UNSIGNED_BYTE, m_image.getImageData()));
  }

  OpenGlTexture2D::~OpenGlTexture2D()
  {
    glCall(glDeleteTextures(1, &m_renderer_id));
  }

  void OpenGlTexture2D::bind(uint8_t slot) const 
  {
    glCall(glActiveTexture(GL_TEXTURE0 + slot));
    glCall(glBindTexture(GL_TEXTURE_2D, m_renderer_id));
  }

}
