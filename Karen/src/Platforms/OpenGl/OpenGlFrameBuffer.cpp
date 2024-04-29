#include "pch.h"
#include "OpenGlFrameBuffer.h"
#include "OpenGlCore.h"
#include "Karen/Render/API/RendererCapabilities.h"


namespace Karen
{
  OpenGlFrameBuffer::OpenGlFrameBuffer(const Specs& specs)
    : m_specs(specs), MAX_FRAMEBUFFER_SIZE(sqrt(Karen::RendererCapabilities::getMaxTextureBufferSize())), 
    MIN_FRAMEBUFFER_SIZE(1)
  {
    KAREN_CORE_WARN("{0}, {1}",MIN_FRAMEBUFFER_SIZE, MAX_FRAMEBUFFER_SIZE);
    reCreate(); 
  }

  OpenGlFrameBuffer::~OpenGlFrameBuffer()
  {
    glCall(glDeleteFramebuffers(1, &m_renderer_id));
    glCall(glDeleteTextures(1, &m_color_attachment));
    glCall(glDeleteTextures(1, &m_depth_attachment));
  }

  void OpenGlFrameBuffer::reCreate()
  {
    if(m_renderer_id)
    {
      glCall(glDeleteFramebuffers(1, &m_renderer_id));
      glCall(glDeleteTextures(1, &m_color_attachment));
      glCall(glDeleteTextures(1, &m_depth_attachment));
    }

    glCall(glGenFramebuffers(1, &m_renderer_id));
    glCall(glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id));

    glCall(glGenTextures(1, &m_color_attachment));
    glCall(glBindTexture(GL_TEXTURE_2D, m_color_attachment));
    glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_specs.width, m_specs.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_attachment, 0));

    glCall(glGenTextures(1, &m_depth_attachment));
    glCall(glBindTexture(GL_TEXTURE_2D, m_depth_attachment));
    glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_specs.width, m_specs.height, 0, GL_DEPTH_STENCIL, 
        GL_UNSIGNED_INT_24_8, nullptr));

    glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth_attachment, 0));

    KAREN_CORE_ASSERT_MSG(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Incomplete FrameBuffer");

    glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
  }

  void OpenGlFrameBuffer::reSize(uint32_t w, uint32_t h)
  {
    if(w > MAX_FRAMEBUFFER_SIZE || h > MAX_FRAMEBUFFER_SIZE)
    {
      KAREN_CORE_WARN("Attempting to resize FrameBuffer to ({0}, {1}), which is greater than MAX_FRAMEBUFFER_SIZE which is ({2}) falling back to the previous FrameBuffer size which is ({3}, {4})", w, h, MAX_FRAMEBUFFER_SIZE, m_specs.width, m_specs.height);
      return;
    }
    else if(w == 0 || h == 0)
    {
      KAREN_CORE_WARN("Attempting to resize FrameBuffer to ({0}, {1}), which is less than MIN_FRAMEBUFFER_SIZE which is ({2}) clamping the value to MIN_FRAMEBUFFER_SIZE", w, h, MIN_FRAMEBUFFER_SIZE);
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
}
