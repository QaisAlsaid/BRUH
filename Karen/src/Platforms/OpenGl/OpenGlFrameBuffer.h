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

    void         reCreate()                   override;
    void         bind()                 const override;
    void         unbind()               const override;
    
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
    uint32_t m_renderer_id;
    uint32_t m_color_attachment;
    uint32_t m_depth_attachment;
    Specs    m_specs;
  };
}

#endif //OPENGL_FRAME_BUFFER_H
