#ifndef OPENGL_VERTEX_BUFFER_H
#define OPENGL_VERTEX_BUFFER_H

#include "Real-Engine/Render/API/BufferLayout.h"
#include "Real-Engine/Render/API/VertexBuffer.h"

namespace Real 
{
  class OpenGlVertexBuffer : public VertexBuffer
  {
  public:
    OpenGlVertexBuffer(uint32_t size, const float* data = nullptr);
    virtual ~OpenGlVertexBuffer();

    void bind()   const override;
    void unbind() const override; 
    void setData(uint32_t size, const void* data) override;
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
