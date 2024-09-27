#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "Real-Engine/Core/Core.h"
#include "Texture.h"
#include "glm/fwd.hpp"

#include <vector>


namespace Real
{
  class REAL_API FrameBuffer
  {
  public:
    enum class TextureInternalFormat
    {
      None = 0,

      RGBA8, RGB32Ui,
      Depth24Stencil8,
      Uint32, Int32
    };
    enum class TextureFormat
    {
      None = 0,

      RGB, RGBA, RGBInt,
      DepthStencil,
      RedInt
    };
    struct TextureSpecs
    {
      TextureSpecs() = default;
      TextureSpecs(TextureFormat fmt, const std::string& name)
        : format(fmt), name(name) {}
      TextureSpecs(TextureFormat fmt, TextureInternalFormat internal, const std::string& name)
        : format(fmt), internal_format(internal), name(name) {}
      TextureSpecs(TextureFormat fmt, TextureInternalFormat internal, Texture::Filters fils, const std::string& name)
        : format(fmt), internal_format(internal), filters(fils), name(name) {}
      
      TextureFormat format = TextureFormat::None;
      TextureInternalFormat internal_format = TextureInternalFormat::None;
      Texture::Filters filters;
  
      std::string name;
    };
    struct AttachmentsSpecs
    {
      AttachmentsSpecs() = default;
      AttachmentsSpecs(const std::initializer_list<TextureSpecs>& specs)
        : attachments(specs) {}
      AttachmentsSpecs(const std::vector<TextureSpecs>& specs)
        : attachments(specs) {}

      std::vector<TextureSpecs> attachments;
    };
    struct Specs
    {
      uint32_t width, height, samples = 1;
      bool is_swap_chain_target = false;
      AttachmentsSpecs attachment_specs;
    };
  public:
    virtual ~FrameBuffer() = default;
    static ARef<FrameBuffer> create(const Specs& specs);
    
    virtual void         reCreate()                              = 0;
    virtual void         reSize(uint32_t width, uint32_t height) = 0;
    virtual void         setSpecs(const Specs& specs) = 0;
    virtual const Specs& getSpecs()                        const = 0;
    virtual void         bind()                            const = 0;
    virtual void         unbind()                          const = 0;
    virtual uint32_t     getColorAttachmentId(const std::string& name)  const = 0;
    virtual int readPixelI(uint32_t attachment, int x, int y) const = 0;
    virtual uint32_t readPixelUi(uint32_t attachment, int x, int y) const = 0;
    virtual glm::uvec3 readPixelV3ui(uint32_t attachment, int x, int y) const = 0;
    virtual void bindWriteFb(uint8_t att) = 0;
  };
}


#endif //FRAME_BUFFER_H
