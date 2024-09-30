#ifndef OPENGL_VERTEX_ARRAY_H
#define OPENGL_VERTEX_ARRAY_H

#include "Real-Engine/Render/API/IndexBuffer.h"
#include "Real-Engine/Render/API/VertexArray.h"
#include "Real-Engine/Core/Core.h"
#include "Real-Engine/Render/API/VertexBuffer.h"


namespace Real
{
  class OpenGlVertexArray : public VertexArray
  {
  public:
    OpenGlVertexArray();
    ~OpenGlVertexArray();

    void bind()   const override;
    void unbind() const override;

    void addVertexBuffer(const Real::ARef<VertexBuffer>&) override;
    void setIndexBuffer(const Real::ARef<IndexBuffer>&)   override;

    const std::vector<Real::ARef<VertexBuffer>>& getVertexBuffers() override;
    const Real::ARef<IndexBuffer>& getIndexBuffer() override;
  private:
    Real::ARef<IndexBuffer>               m_index_buff;
    std::vector<Real::ARef<VertexBuffer>> m_vertex_buffs;
    uint32_t m_vertex_buff_idx = 0;
    uint32_t m_renderer_id = 0;
  };
}

#endif //OPENGL_VERTEX_ARRAY_H
