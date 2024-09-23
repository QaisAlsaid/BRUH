#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <cstdint>
#include "Karen/Core/Core.h"

namespace Karen
{
  class KAREN_API IndexBuffer
  {
  public:
    virtual ~IndexBuffer();

    virtual void bind()   const = 0;
    virtual void unbind() const = 0;
    virtual void setData(uint32_t count, const uint32_t* data, uint16_t usage) = 0;
    virtual uint32_t getCount() const = 0;
    static ARef<IndexBuffer> create(uint32_t count, const uint32_t* data, uint16_t usage);
  };
}

#endif //INDEX_BUFFER_H
