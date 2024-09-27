#include "pch.h"
#include "FrameBuffer.h"
#include "RendererAPI.h"
#include "Platforms/OpenGl/OpenGlFrameBuffer.h"

namespace Real
{
  ARef<FrameBuffer> FrameBuffer::create(const Specs& specs)
  {
    switch(RendererAPI::getAPI())
    {
      case RendererAPI::API::OpenGl:
      {
        return Real::createARef<OpenGlFrameBuffer>(specs);
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
