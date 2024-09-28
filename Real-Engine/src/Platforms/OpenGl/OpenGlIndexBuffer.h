#ifndef OPENGL_INDEX_BUFFER_H
#define OPENGL_INDEX_BUFFER_H

#include "Real-Engine/Render/API/IndexBuffer.h"

namespace Real 
{
  class OpenGlIndexBuffer : public IndexBuffer
  {
  public:
    OpenGlIndexBuffer(uint32_t count, const uint32_t* data, uint16_t usage);
    virtual ~OpenGlIndexBuffer();

    void bind()   const override;
    void unbind() const override; 
    void setData(uint32_t count, const unsigned* data, uint16_t usage) override;
    uint32_t inline getCount() const override {return m_count;}
  private:
    uint32_t m_renderer_id;
    uint32_t m_count = 0;
  };
}


#endif //OPENGL_VERTEX_BUFFER_H
