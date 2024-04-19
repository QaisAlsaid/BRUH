#ifndef OPENGL_VERTEX_ARRAY_H
#define OPENGL_VERTEX_ARRAY_H

#include "Karen/Render/API/IndexBuffer.h"
#include "Karen/Render/API/VertexArray.h"
#include "Karen/Core/Core.h"
#include "Karen/Render/API/VertexBuffer.h"
namespace Karen
{
  class OpenGlVertexArray : public VertexArray
  {
  public:
    OpenGlVertexArray();
    ~OpenGlVertexArray();

    void bind()   const override;
    void unbind() const override;

    void addVertexBuffer(const Karen::ARef<VertexBuffer>&) override;
    void setIndexBuffer(const Karen::ARef<IndexBuffer>&)   override;

    const std::vector<Karen::ARef<VertexBuffer>>& getVertexBuffers() override;
    const Karen::ARef<IndexBuffer>& getIndexBuffer() override;
  private:
    Karen::ARef<IndexBuffer>               m_index_buff;
    std::vector<Karen::ARef<VertexBuffer>> m_vertex_buffs;
    uint32_t m_renderer_id = 0;
  };
}

#endif //OPENGL_VERTEX_ARRAY_H
