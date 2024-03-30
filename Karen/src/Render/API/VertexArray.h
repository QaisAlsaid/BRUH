#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "Karen/Core.h"
#include "Render/API/IndexBuffer.h"
#include "Render/API/VertexBuffer.h"
#include <memory>

namespace Karen
{
  class VertexArray
  {
  public:
    virtual ~VertexArray() {};

    virtual void bind()   const = 0;
    virtual void unbind() const = 0;

    virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>&) = 0;
    virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>&)   = 0;

    virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() = 0;
    virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() = 0;
  
    static VertexArray* create();
  };
}


#endif //VERTEX_ARRAY_H
