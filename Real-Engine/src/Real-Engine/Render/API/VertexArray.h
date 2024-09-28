#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "Real-Engine/Core/Core.h"
#include "Real-Engine/Render/API/IndexBuffer.h"
#include "Real-Engine/Render/API/VertexBuffer.h"
#include <memory>

namespace Real
{
  class VertexArray
  {
  public:
    virtual ~VertexArray() {};

    virtual void bind()   const = 0;
    virtual void unbind() const = 0;

    virtual void addVertexBuffer(const Real::ARef<VertexBuffer>&) = 0;
    virtual void setIndexBuffer(const Real::ARef<IndexBuffer>&)   = 0;

    virtual const std::vector<Real::ARef<VertexBuffer>>& getVertexBuffers() = 0;
    virtual const Real::ARef<IndexBuffer>& getIndexBuffer() = 0;
  
    static ARef<VertexArray> create();
  };
}


#endif //VERTEX_ARRAY_H
