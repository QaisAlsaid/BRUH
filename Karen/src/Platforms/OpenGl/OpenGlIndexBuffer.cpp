#include "pch.h"
#include "Platforms/OpenGl/OpenGlIndexBuffer.h"
#include "Platforms/OpenGl/OpenGlCore.h"


namespace Karen
{
  OpenGlIndexBuffer::OpenGlIndexBuffer(uint32_t count, const uint32_t* data, uint16_t usage)
  {
    glGenBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
    setData(count, data, usage);
    m_count = count;
  }

  OpenGlIndexBuffer::~OpenGlIndexBuffer()
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    delete m_data;
    glDeleteBuffers(1, &m_renderer_id);
  }

  void OpenGlIndexBuffer::bind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
  }

  void OpenGlIndexBuffer::unbind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  
  void OpenGlIndexBuffer::setData(uint32_t count, const uint32_t* data, uint16_t usage)
  {
    //TODO:make the usage work:
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
    m_data = new uint32_t[count];
    memcpy(m_data, data, count);
    m_count = count;
  }
}
