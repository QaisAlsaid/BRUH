#include "pch.h"
#include "Real-Engine/Render/API/IndexBuffer.h"
#include "Platforms/OpenGl/OpenGlVertexArray.h"
#include "Platforms/OpenGl/OpenGlCore.h"


namespace Real
{
  OpenGlVertexArray::OpenGlVertexArray()
  {
    REAL_PROFILE_FUNCTION();
    glGenVertexArrays(1, &m_renderer_id); 
  }

  OpenGlVertexArray::~OpenGlVertexArray()
  {
    REAL_PROFILE_FUNCTION();
    glDeleteVertexArrays(1, &m_renderer_id);
  }

  void OpenGlVertexArray::bind() const
  {
    REAL_PROFILE_FUNCTION();
    glBindVertexArray(m_renderer_id);
  }

  void OpenGlVertexArray::unbind() const
  {
    REAL_PROFILE_FUNCTION();
    glBindVertexArray(0);
  }

  void OpenGlVertexArray::addVertexBuffer(const Real::ARef<VertexBuffer>& vb)
  {
    REAL_PROFILE_FUNCTION();
    vb->bind();
    const auto& bl = vb->getLayout();
    for(uint32_t i = 0; i < bl.getElements().size(); ++i)
    {
      glEnableVertexAttribArray(i);
      glVertexAttribPointer(i,
      bl.getElements().at(i).count, GL_FLOAT,
      bl.getElements().at(i).normalized == true ? GL_TRUE : GL_FALSE,
      bl.getStride(), (const void*)(uintptr_t)(bl.getElements().at(i).offset));
    }
    m_vertex_buffs.push_back(vb);
    vb->unbind();
    glBindVertexArray(0);
  }

  void OpenGlVertexArray::setIndexBuffer(const Real::ARef<IndexBuffer>& ib)
  {
    REAL_PROFILE_FUNCTION();
    glBindVertexArray(m_renderer_id);
    m_index_buff = ib;
    ib->bind();
  }

  const Real::ARef<IndexBuffer>& OpenGlVertexArray::getIndexBuffer()
  {
    return m_index_buff;
  }

  const std::vector<Real::ARef<VertexBuffer>>& OpenGlVertexArray::getVertexBuffers()
  {
    return m_vertex_buffs;
  }
}
