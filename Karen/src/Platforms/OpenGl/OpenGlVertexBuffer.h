#ifndef OPENGL_VERTEX_BUFFER_H
#define OPENGL_VERTEX_BUFFER_H

#include "Karen/Render/API/BufferLayout.h"
#include "Karen/Render/API/VertexBuffer.h"

namespace Karen 
{
  class OpenGlVertexBuffer : public VertexBuffer
  {
  public:
    OpenGlVertexBuffer(uint32_t size, const float* data, uint16_t usage);
    virtual ~OpenGlVertexBuffer();

    void bind()   const override;
    void unbind() const override; 
    void setData(uint32_t size, const float* data, uint16_t usage) override;
    inline void setLayout(const BufferLayout& layout) override
    {
      m_layout = layout;
    }
    inline const BufferLayout& getLayout() const override
    {
      return m_layout;
    }
  private:
    uint32_t m_renderer_id;
    BufferLayout m_layout;
  };
}


#endif //OPENGL_VERTEX_BUFFER_H
