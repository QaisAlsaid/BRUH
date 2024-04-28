#ifndef OPENGL_FRAME_BUFFER_H
#define OPENGL_FRAME_BUFFER_H

#include "Karen/Render/API/FrameBuffer.h"

namespace Karen
{
  class OpenGlFrameBuffer : public FrameBuffer
  {
  public:
    OpenGlFrameBuffer(const Specs& specs);
    ~OpenGlFrameBuffer();

    void reCreate()                                   override;
    void reSize(uint32_t width, uint32_t height)      override;     
    void bind()                                 const override;
    void unbind()                               const override;
    
    uint32_t getColorAttachmentId() const override
    {
      return m_color_attachment;
    }
    void setSpecs(const Specs& specs) override
    {
      m_specs = specs;
    }
    const Specs& getSpecs() const override
    {
      return m_specs;
    }

  private:
    uint32_t m_renderer_id      = 0;
    uint32_t m_color_attachment = 0;
    uint32_t m_depth_attachment = 0;
    Specs    m_specs;
  };
}

#endif //OPENGL_FRAME_BUFFER_H
