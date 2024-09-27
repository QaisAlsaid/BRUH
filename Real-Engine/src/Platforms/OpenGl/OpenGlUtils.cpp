#include "OpenGlUtils.h"


namespace Real::OpenGlUtils
{
  GLenum toOpenGlFilters(Texture::FilterType ft)
  {
    switch(ft)
    {
      case Texture::FilterType::Linear: return GL_LINEAR;
      case Texture::FilterType::Nearest: return GL_NEAREST;
    }
    REAL_CORE_ASSERT(false);
    return 0;
  }

  GLenum toOpenGlWrapModes(Texture::WrapMode wm)
  {
    switch(wm)
    {
      case Texture::WrapMode::ClampToEdge: return GL_CLAMP_TO_EDGE;
      case Texture::WrapMode::ClampToBorder: return GL_CLAMP_TO_BORDER;
      case Texture::WrapMode::Repeat: return GL_REPEAT;
    }
    REAL_CORE_ASSERT(false);
    return 0;
  }

  GLenum toOpenGlFormat(FrameBuffer::TextureFormat fptf)
  {
    switch(fptf)
    {
      case FrameBuffer::TextureFormat::RGB: return GL_RGB;
      case FrameBuffer::TextureFormat::RGBA: return GL_RGBA;
      case FrameBuffer::TextureFormat::RedInt: return GL_RED_INTEGER;
      case FrameBuffer::TextureFormat::DepthStencil: return GL_DEPTH_STENCIL;
      case FrameBuffer::TextureFormat::RGBInt: return GL_RGB_INTEGER;
      case FrameBuffer::TextureFormat::None: REAL_CORE_ERROR("Unsupported"); return 0;
    }
    REAL_CORE_ASSERT(false);
    return 0;
  }

  GLenum toOpenGlFormat(FrameBuffer::TextureInternalFormat i_fmt)
  {
    switch(i_fmt)
    {
      case FrameBuffer::TextureInternalFormat::RGBA8: return GL_RGBA8;
      case FrameBuffer::TextureInternalFormat::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
      case FrameBuffer::TextureInternalFormat::RGB32Ui: return GL_RGB32UI;
      case FrameBuffer::TextureInternalFormat::Int32: return GL_R32I;
      case FrameBuffer::TextureInternalFormat::Uint32: return GL_R32UI;
      case FrameBuffer::TextureInternalFormat::None: REAL_CORE_ERROR("Unsupported"); return 0;
    }
    REAL_CORE_ASSERT(false);
    return 0;
  }

  void bindTexture(GLuint id, int samples)
  {
    if(samples > 1)
    {
      glCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id));
    }
    else
    {
      glCall(glBindTexture(GL_TEXTURE_2D, id));
    }
  }

  void applayFilters(const Texture::Filters& fils)
  {
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toOpenGlFilters(fils.min_filter)));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toOpenGlFilters(fils.mag_filter)));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, toOpenGlWrapModes(fils.wrap_s));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, toOpenGlWrapModes(fils.wrap_r));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, toOpenGlWrapModes(fils.wrap_t));
  }
}
