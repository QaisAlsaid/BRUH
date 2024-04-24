#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "Karen/Core/Core.h"


namespace Karen
{
  class FrameBuffer
  {
  public:
    struct Specs
    {
      uint32_t width, height, samples = 1;
      bool is_swap_chain_target = false;
    };
  public:
    virtual ~FrameBuffer() = default;
    static ARef<FrameBuffer> create(const Specs& specs);
    
    virtual void         reCreate()                   = 0;
    virtual void         setSpecs(const Specs& specs) = 0;
    virtual const Specs& getSpecs()             const = 0;
    virtual void         bind()                 const = 0;
    virtual void         unbind()               const = 0;
    virtual uint32_t     getColorAttachmentId() const = 0;
  };
}


#endif //FRAME_BUFFER_H
