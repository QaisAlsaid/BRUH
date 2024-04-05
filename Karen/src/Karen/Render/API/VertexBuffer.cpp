#include "pch.h"
#include "Karen/Render/API/VertexBuffer.h"
#include "Karen/Core/Core.h"
#include "Karen/Render/API/RendererAPI.h"
#include "Platforms/OpenGl/OpenGlVertexBuffer.h"

namespace Karen
{
  VertexBuffer::~VertexBuffer()
  {

  }

  VertexBuffer* VertexBuffer::create(uint32_t size, const float* data, uint16_t usage)
  {
    switch(RendererAPI::getAPI())
    {
      case RendererAPI::API::OpenGl:
      {
        return new OpenGlVertexBuffer(size, data, usage);
        break;
      }
      case RendererAPI::API::NONE:
      {
        return nullptr;
        KAREN_CORE_ASSERT(false, "Not supported RenderAPI: 'RenderAPI:NONE' at the moment");
        break;
      }
    }
    KAREN_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
  }
}
