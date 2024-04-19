#include "pch.h"
#include "Karen/Render/API/IndexBuffer.h"
#include "Karen/Core/Core.h"
#include "Platforms/OpenGl/OpenGlIndexBuffer.h"
#include "Karen/Render/API/RendererAPI.h"

namespace Karen
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
        return Karen::createARef<OpenGlIndexBuffer>(count, data, usage);
        break;
      }
      case RendererAPI::API::NONE:
      {
        return nullptr;
        KAREN_CORE_ASSERT_MSG(false, "Not supported RenderAPI: 'RenderAPI:NONE' at the moment");
        break;
      }
    }
    KAREN_CORE_ASSERT_MSG(false, "Unknown RendererAPI");
    return nullptr;
  }
}
