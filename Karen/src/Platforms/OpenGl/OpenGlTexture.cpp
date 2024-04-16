#include "pch.h"
#include "Platforms/OpenGl/OpenGlTexture.h"
#include "Platforms/OpenGl/OpenGlCore.h"


namespace Karen
{
  OpenGlTexture2D::OpenGlTexture2D(const std::string& file_path, bool flip_v)
  {
    glCall(glGenTextures(1, &m_renderer_id));
    glCall(glActiveTexture(GL_TEXTURE0));
    glCall(glBindTexture(GL_TEXTURE_2D, m_renderer_id));

    bool status = m_image.loadFromFile(file_path, flip_v);
    KAREN_CORE_ASSERT(status, std::string("Failed to load Texture from file: ") + file_path);
    uint32_t fmt = 0;
    switch(m_image.getColorChannels())
    {
      case 3:
        fmt = GL_RGB;
        break;
      case 4:
        fmt = GL_RGBA;
        break;
    }
    m_internal_format = fmt;
    m_data_format = fmt;
    KAREN_CORE_ASSERT(m_image.getColorChannels() > 2 && m_image.getColorChannels() < 5, "Unsupported Image format");
    
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)); 
    glCall(glTexImage2D(GL_TEXTURE_2D, 0, fmt, m_image.getWidth(), m_image.getHeight()
        , 0, fmt, GL_UNSIGNED_BYTE, m_image.getImageData()));
    m_width = m_image.getWidth();
    m_height = m_image.getHeight();
    m_image.~ImageLoader();
  }

  OpenGlTexture2D::OpenGlTexture2D(uint32_t w, uint32_t h, size_t ds, const void* d)
  {
    m_internal_format = GL_RGBA;
    m_data_format = GL_RGBA;
    m_width = w;
    m_height = h;
    glCall(glGenTextures(1, &m_renderer_id));
    glCall(glActiveTexture(GL_TEXTURE0));
    glCall(glBindTexture(GL_TEXTURE_2D, m_renderer_id));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)); 
    if(ds != 0 && d) setData(ds, d);
  }
  
  void OpenGlTexture2D::bind(uint8_t slot) const 
  {
    glCall(glActiveTexture(GL_TEXTURE0 + slot));
    glCall(glBindTexture(GL_TEXTURE_2D, m_renderer_id));
  }

  void OpenGlTexture2D::setData(size_t ds, const void* d)
  {
    bind(0);
    glCall(glTexImage2D(GL_TEXTURE_2D, 0, (GLenum) m_data_format, m_width, m_height
        , 0, (GLenum) m_internal_format, GL_UNSIGNED_BYTE, d));
  }

  OpenGlTexture2D::~OpenGlTexture2D()
  {
    glCall(glDeleteTextures(1, &m_renderer_id));
  }
 }
