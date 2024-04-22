#include "pch.h"
#include "Platforms/OpenGl/OpenGlVertexBuffer.h"
#include "Platforms/OpenGl/OpenGlCore.h"


namespace Karen
{
  OpenGlVertexBuffer::OpenGlVertexBuffer(uint32_t size, const float* data)
  {
    KAREN_PROFILE_FUNCTION();
    glGenBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
    if(data != nullptr)
      glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    else
      glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

  }

  OpenGlVertexBuffer::~OpenGlVertexBuffer()
  {
    KAREN_PROFILE_FUNCTION();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &m_renderer_id);
  }

  void OpenGlVertexBuffer::bind() const
  {
    KAREN_PROFILE_FUNCTION();
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
  }

  void OpenGlVertexBuffer::unbind() const
  {
    KAREN_PROFILE_FUNCTION();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void OpenGlVertexBuffer::setData(uint32_t size, const void* data)
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
  }
}
