#include "pch.h"
#include "Real-Engine/Render/API/IndexBuffer.h"
#include "Real-Engine/Core/Core.h"
#include "Platforms/OpenGl/OpenGlIndexBuffer.h"
#include "Real-Engine/Render/API/RendererAPI.h"

namespace Real 
{
  IndexBuffer::~IndexBuffer()
  {

  }

  ARef<IndexBuffer> IndexBuffer::create(uint32_t count, const uint32_t* data, uint16_t usage)
  {
    switch(RendererAPI::getAPI())
    {
      case RendererAPI::API::OpenGl:
      {
        return Real::createARef<OpenGlIndexBuffer>(count, data, usage);
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
}
