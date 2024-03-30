#include "Render/API/IndexBuffer.h"
#include "pch.h"
#include "Platforms/OpenGl/OpenGlVertexArray.h"
#include <memory>
#include <glad/glad.h>

namespace Karen
{
  OpenGlVertexArray::OpenGlVertexArray()
  {
    glGenVertexArrays(1, &m_renderer_id); 
  }

  OpenGlVertexArray::~OpenGlVertexArray()
  {
    glDeleteVertexArrays(1, &m_renderer_id);
  }

  void OpenGlVertexArray::bind() const
  {
    glBindVertexArray(m_renderer_id);
  }

  void OpenGlVertexArray::unbind() const
  {
    glBindVertexArray(0);
  }

  void OpenGlVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vb)
  {
    vb->bind();
    const auto& bl = vb->getLayout();
    for(int i = 0; i < bl.getElements().size(); ++i)
    {
      glEnableVertexAttribArray(i);
      glVertexAttribPointer(i,
      bl.getElements().at(i).count, GL_FLOAT,
      bl.getElements().at(i).normalized == true ? GL_TRUE : GL_FALSE,
      bl.getStride(), (const void*)bl.getElements().at(i).offset);
    }
    m_vertex_buffs.push_back(vb);
    vb->unbind();
    glBindVertexArray(0);
  }

  void OpenGlVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& ib)
  {
    glBindVertexArray(m_renderer_id);
    m_index_buff = ib;
    ib->bind();
  }

  const std::shared_ptr<IndexBuffer>& OpenGlVertexArray::getIndexBuffer()
  {
    return m_index_buff;
  }

  const std::vector<std::shared_ptr<VertexBuffer>>& OpenGlVertexArray::getVertexBuffers()
  {
    return m_vertex_buffs;
  }
}