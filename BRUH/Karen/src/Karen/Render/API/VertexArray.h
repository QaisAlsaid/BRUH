#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "Karen/Core/Core.h"
#include "Karen/Render/API/IndexBuffer.h"
#include "Karen/Render/API/VertexBuffer.h"
#include <memory>

namespace Karen
{
  class VertexArray
  {
  public:
    virtual ~VertexArray() {};

    virtual void bind()   const = 0;
    virtual void unbind() const = 0;

    virtual void addVertexBuffer(const Karen::ARef<VertexBuffer>&) = 0;
    virtual void setIndexBuffer(const Karen::ARef<IndexBuffer>&)   = 0;

    virtual const std::vector<Karen::ARef<VertexBuffer>>& getVertexBuffers() = 0;
    virtual const Karen::ARef<IndexBuffer>& getIndexBuffer() = 0;
  
    static ARef<VertexArray> create();
  };
}


#endif //VERTEX_ARRAY_H
