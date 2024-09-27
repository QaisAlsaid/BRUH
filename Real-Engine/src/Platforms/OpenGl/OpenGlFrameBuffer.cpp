#include "Real-Engine/Render/API/FrameBuffer.h"
#include "glm/fwd.hpp"
#include "pch.h"
#include "OpenGlFrameBuffer.h"
#include "OpenGlCore.h"
#include "Real-Engine/Render/API/RendererCapabilities.h"
#include "Real-Engine/Core/Math/math.h"
#include "OpenGlUtils.h"


namespace Real
{
  static bool isDepthFormat(FrameBuffer::TextureFormat fmt)
  {
    switch(fmt)
    {
      case FrameBuffer::TextureFormat::DepthStencil: return true;
      default: return false;
    }
  }

  static uint32_t genTexture(int samples, const FrameBuffer::TextureSpecs& specs, uint32_t width, uint32_t height, uint32_t type = 0)
  {
    uint32_t id;
    glCall(glGenTextures(1, &id));
    if(samples > 1)
    {
      glCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id));
      glCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, OpenGlUtils::toOpenGlFormat(specs.format), width, height, GL_FALSE));
    }
    else 
    {
      glCall(glBindTexture(GL_TEXTURE_2D, id));
      
      glCall(glTexImage2D(GL_TEXTURE_2D, 0, OpenGlUtils::toOpenGlFormat(specs.internal_format), width, height, 0, OpenGlUtils::toOpenGlFormat(specs.format), type, nullptr));
    }
    return id;
  }

  static void attachTexture(int samples, uint32_t id, int index)
  {
    if(samples > 1)
    {
      glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D_MULTISAMPLE, id, 0));
    }
    else 
    {
      glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, id, 0));
    }
  }

  static void attachDepth(int samples, uint32_t id, FrameBuffer::TextureFormat fmt, uint32_t depth_type)
  {
    if(samples > 1)
    {
      glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, depth_type, GL_TEXTURE_2D_MULTISAMPLE, id, 0));
    }
    else 
    {
      glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, depth_type, GL_TEXTURE_2D, id, 0));
    }
  }

  OpenGlFrameBuffer::OpenGlFrameBuffer(const Specs& specs)
    : m_specs(specs), MAX_FRAMEBUFFER_SIZE(sqrt(Real::RendererCapabilities::getMaxTextureBufferSize())), 
    MIN_FRAMEBUFFER_SIZE(1)
  {
    reCreate();
  }

  OpenGlFrameBuffer::~OpenGlFrameBuffer()
  {
    glCall(glDeleteFramebuffers(1, &m_renderer_id));
    glCall(glDeleteTextures(1, &m_depth_attachment_id));
    for(const auto& it : m_color_attachments_ids)
    {
      glCall(glDeleteTextures(1, &it.second));
    }
  }

  void OpenGlFrameBuffer::reCreate()
  {
    if(m_renderer_id)
    {
      glCall(glDeleteFramebuffers(1, &m_renderer_id));
      glCall(glDeleteTextures(1, &m_depth_attachment_id));
      for(const auto& it : m_color_attachments_ids)
      {
        glCall(glDeleteTextures(1, &it.second));
      }

      m_depth_attachment_id = 0;
      m_color_attachments_ids.clear();
    }

    glCall(glGenFramebuffers(1, &m_renderer_id));
    glCall(glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id));

    uint8_t ctr = 0;
    for(auto& cas : m_specs.attachment_specs.attachments)
    {
      if(isDepthFormat(cas.format)) 
      {
        m_depth_attachment = cas;
        continue;
      }
      uint32_t id = 0;
      if(cas.format == FrameBuffer::TextureFormat::RedInt || cas.format == FrameBuffer::TextureFormat::RGBInt)
      {
        id = genTexture(m_specs.samples, cas, m_specs.width, m_specs.height, GL_UNSIGNED_INT);
      }
      else
      {
        id = genTexture(m_specs.samples, cas, m_specs.width, m_specs.height, GL_UNSIGNED_BYTE);
      }
      OpenGlUtils::bindTexture(id, m_specs.samples);
      if(m_specs.samples < 2)
        OpenGlUtils::applayFilters(cas.filters);
      attachTexture(m_specs.samples, id, ctr);
      m_color_attachments_ids[cas.name] = id;
      ++ctr;
    }
    if(m_depth_attachment.format != TextureFormat::None && m_depth_attachment.internal_format != TextureInternalFormat::None)
    {
      m_depth_attachment_id = genTexture(m_specs.samples, m_depth_attachment, m_specs.width, m_specs.height, GL_UNSIGNED_INT_24_8);
      OpenGlUtils::bindTexture(m_depth_attachment_id, m_specs.samples);
      OpenGlUtils::applayFilters(m_depth_attachment.filters);
      attachDepth(m_specs.samples, m_depth_attachment_id, m_depth_attachment.format, GL_DEPTH_STENCIL_ATTACHMENT);
    }
    /*glCall(glGenTextures(1, &m_color_attachment));
    glCall(glBindTexture(GL_TEXTURE_2D, m_color_attachment));
    glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_specs.width, m_specs.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
*/
    //glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_attachment, 0));

    //glCall(glGenTextures(1, &m_depth_attachment_id));
    //glCall(glBindTexture(GL_TEXTURE_2D, m_depth_attachment));
    //glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_specs.width, m_specs.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr));

    //glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth_attachment, 0));

    REAL_CORE_ASSERT_MSG(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Incomplete FrameBuffer");

    glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
  }

  void OpenGlFrameBuffer::reSize(uint32_t w, uint32_t h)
  {
    if(w > MAX_FRAMEBUFFER_SIZE || h > MAX_FRAMEBUFFER_SIZE)
    {
      REAL_CORE_WARN("Attempting to resize FrameBuffer to ({0}, {1}), which is greater than MAX_FRAMEBUFFER_SIZE which is ({2}) falling back to the previous FrameBuffer size which is ({3}, {4})", w, h, MAX_FRAMEBUFFER_SIZE, m_specs.width, m_specs.height);
      return;
    }
    else if(w == 0 || h == 0)
    {
      REAL_CORE_WARN("Attempting to resize FrameBuffer to ({0}, {1}), which is less than MIN_FRAMEBUFFER_SIZE which is ({2}) clamping the value to MIN_FRAMEBUFFER_SIZE", w, h, MIN_FRAMEBUFFER_SIZE);
      m_specs.width  = (w == 0 ? MIN_FRAMEBUFFER_SIZE : w);
      m_specs.height = (h == 0 ? MIN_FRAMEBUFFER_SIZE : h);
      reCreate();
    }
    else [[likely]]
    {
      m_specs.width  = w;
      m_specs.height = h;
      reCreate();
    }
  }

  void OpenGlFrameBuffer::bind() const
  {
    glCall(glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id));
  }

  void OpenGlFrameBuffer::unbind() const
  {
    glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
  }

  int OpenGlFrameBuffer::readPixelI(uint32_t att, int x, int y) const
  {
    REAL_CORE_ASSERT(m_color_attachments_ids.size() > att);
    int out_val = 0;
    glReadBuffer(GL_COLOR_ATTACHMENT0 + att);
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &out_val);
    return out_val;

  }

  uint32_t OpenGlFrameBuffer::readPixelUi(uint32_t att, int x, int y) const
  {
    REAL_CORE_ASSERT(m_color_attachments_ids.size() > att);
    uint32_t out_val = 0u;
    glReadBuffer(GL_COLOR_ATTACHMENT0 + att);
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &out_val);
    return out_val;

  }

  glm::uvec3 OpenGlFrameBuffer::readPixelV3ui(uint32_t att, int x, int y) const
  {
    REAL_CORE_ASSERT(m_color_attachments_ids.size() > att);
    glm::uvec3 out_val(0u);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + att);
    glReadPixels(x, y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &out_val);
    return out_val;
  }

  void OpenGlFrameBuffer::bindWriteFb(uint8_t id)
  {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_renderer_id);
  }

  /*
  void OpenGlFrameBuffer::clearColorAttachment(uint32_t att, int val)
  {
    glGetFloatv(GL_COLOR_CLEAR_VALUE, prev_clear)
    const auto& spec = m_specs.specs.attachments.at(att);
    glClearBuffer();
  }
  */
}
