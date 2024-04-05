#include "pch.h"
#include "Karen/Render/API/BufferLayout.h"
#include "Platforms/OpenGl/OpenGlVertexBuffer.h"

#ifndef KAREN_EMSCRIPTEN
#include <glad/glad.h>
#else 
#include <GLES3/gl32.h>
#endif //KAREN_EMSCRIPTEN

namespace Karen
{
  OpenGlVertexBuffer::OpenGlVertexBuffer(uint32_t size, const float* data, uint16_t usage)
  {
    glGenBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
    setData(size, data, usage);
  }

  OpenGlVertexBuffer::~OpenGlVertexBuffer()
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    delete m_data;
    glDeleteBuffers(1, &m_renderer_id);
  }

  void OpenGlVertexBuffer::bind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
  }

  void OpenGlVertexBuffer::unbind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void OpenGlVertexBuffer::setData(uint32_t size, const float* data, uint16_t usage)
  {
    //TODO: make the usage work
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    m_data = (float*)new char[size];
    memcpy(m_data, data, size);
  }
}
