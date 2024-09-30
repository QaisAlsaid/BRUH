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
      switch(bl.getElements().at(i).type)
      {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
        {
          glEnableVertexAttribArray(m_vertex_buff_idx);
          glVertexAttribPointer(m_vertex_buff_idx,
          bl.getElements().at(i).count, GL_FLOAT,
          bl.getElements().at(i).normalized == true ? GL_TRUE : GL_FALSE,
          bl.getStride(), (const void*)(uintptr_t)(bl.getElements().at(i).offset));
          m_vertex_buff_idx++;
          break;
        }
        case ShaderDataType::Sampler2D:
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
        {
          glEnableVertexAttribArray(m_vertex_buff_idx);
          glVertexAttribIPointer(m_vertex_buff_idx,
          bl.getElements().at(i).count, GL_INT,
          bl.getStride(), (const void*)(uintptr_t)(bl.getElements().at(i).offset));
          m_vertex_buff_idx++;
          break;
        }
        case ShaderDataType::Mat2:
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
        case ShaderDataType::None:
        default: REAL_CORE_ERROR("Unsupported ShaderDataType: {0}", ShaderDataTypeToString(bl.getElements().at(i).type)); break;
      }
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
