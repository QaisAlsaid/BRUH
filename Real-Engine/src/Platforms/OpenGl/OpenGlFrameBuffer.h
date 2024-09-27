#ifndef OPENGL_FRAME_BUFFER_H
#define OPENGL_FRAME_BUFFER_H

#include "Real-Engine/Render/API/FrameBuffer.h"
#include "glm/fwd.hpp"

namespace Real
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
    int readPixelI(uint32_t att, int x, int y) const override; 
    uint32_t readPixelUi(uint32_t att, int x, int y) const override; 
    glm::uvec3 readPixelV3ui(uint32_t att, int x, int y) const override; 
    void bindWriteFb(uint8_t aat) override;
    uint32_t getColorAttachmentId(const std::string& name) const override
    {
      return m_color_attachments_ids.at(name);
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
    uint32_t                  m_renderer_id       = 0;
    TextureSpecs              m_depth_attachment;
    std::unordered_map<std::string, uint32_t> m_color_attachments_ids;
    uint32_t m_depth_attachment_id = 0;
    Specs                 m_specs;
    const uint32_t MAX_FRAMEBUFFER_SIZE;
    const uint32_t MIN_FRAMEBUFFER_SIZE;
  };
}

#endif //OPENGL_FRAME_BUFFER_H
