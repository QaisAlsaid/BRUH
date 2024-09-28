#include "pch.h"
#include "Real-Engine/Render/API/VertexBuffer.h"
#include "Real-Engine/Core/Core.h"
#include "Real-Engine/Render/API/RendererAPI.h"
#include "Platforms/OpenGl/OpenGlVertexBuffer.h"

namespace Real
{
   ARef<VertexBuffer> VertexBuffer::create(uint32_t size, const float* data)
  {
    switch(RendererAPI::getAPI())
    {
      case RendererAPI::API::OpenGl:
      {
        return Real::createARef<OpenGlVertexBuffer>(size, data);
        break;
      }
      case RendererAPI::API::NONE:
      {
        return nullptr;
        REAL_CORE_ASSERT_MSG(false, "Not supported RenderAPI: 'RenderAPI:NONE' at the moment");
        break;
      }
    }
    REAL_CORE_ASSERT_MSG(false, "Unknown RendererAPI");
    return nullptr;
  }
   
  VertexBuffer::~VertexBuffer()
  {

  }


}
