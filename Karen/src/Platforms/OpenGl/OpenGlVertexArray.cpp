#include "pch.h"
#include "Karen/Render/API/IndexBuffer.h"
#include "Platforms/OpenGl/OpenGlVertexArray.h"
#include "Platforms/OpenGl/OpenGlCore.h"


namespace Karen
{
  OpenGlVertexArray::OpenGlVertexArray()
  {
    KAREN_PROFILE_FUNCTION();
    glGenVertexArrays(1, &m_renderer_id); 
  }

  OpenGlVertexArray::~OpenGlVertexArray()
  {
    KAREN_PROFILE_FUNCTION();
    glDeleteVertexArrays(1, &m_renderer_id);
  }

  void OpenGlVertexArray::bind() const
  {
    KAREN_PROFILE_FUNCTION();
    glBindVertexArray(m_renderer_id);
  }

  void OpenGlVertexArray::unbind() const
  {
    KAREN_PROFILE_FUNCTION();
    glBindVertexArray(0);
  }

  void OpenGlVertexArray::addVertexBuffer(const Karen::ARef<VertexBuffer>& vb)
  {
    KAREN_PROFILE_FUNCTION();
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

  void OpenGlVertexArray::setIndexBuffer(const Karen::ARef<IndexBuffer>& ib)
  {
    KAREN_PROFILE_FUNCTION();
    glBindVertexArray(m_renderer_id);
    m_index_buff = ib;
    ib->bind();
  }

  const Karen::ARef<IndexBuffer>& OpenGlVertexArray::getIndexBuffer()
  {
    return m_index_buff;
  }

  const std::vector<Karen::ARef<VertexBuffer>>& OpenGlVertexArray::getVertexBuffers()
  {
    return m_vertex_buffs;
  }
}
