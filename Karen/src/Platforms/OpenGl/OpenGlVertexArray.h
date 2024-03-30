#ifndef OPENGL_VERTEX_ARRAY_H
#define OPENGL_VERTEX_ARRAY_H

#include "Render/API/IndexBuffer.h"
#include "Render/API/VertexArray.h"
#include "Karen/Core.h"
#include "Render/API/VertexBuffer.h"
namespace Karen
{
  class OpenGlVertexArray : public VertexArray
  {
  public:
    OpenGlVertexArray();
    ~OpenGlVertexArray();

    void bind()   const override;
    void unbind() const override;

    void addVertexBuffer(const std::shared_ptr<VertexBuffer>&) override;
    void setIndexBuffer(const std::shared_ptr<IndexBuffer>&)   override;

    const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() override;
    const std::shared_ptr<IndexBuffer>& getIndexBuffer() override;
  private:
    std::shared_ptr<IndexBuffer>               m_index_buff;
    std::vector<std::shared_ptr<VertexBuffer>> m_vertex_buffs;
    uint32_t m_renderer_id = 0;
  };
}

#endif //OPENGL_VERTEX_ARRAY_H