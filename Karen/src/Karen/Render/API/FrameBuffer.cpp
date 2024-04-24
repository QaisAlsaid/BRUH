#include "pch.h"
#include "FrameBuffer.h"
#include "RendererAPI.h"
#include "Platforms/OpenGl/OpenGlFrameBuffer.h"

namespace Karen
{
  ARef<FrameBuffer> FrameBuffer::create(const Specs& specs)
  {
    switch(RendererAPI::getAPI())
    {
      case RendererAPI::API::OpenGl:
      {
        return Karen::createARef<OpenGlFrameBuffer>(specs);
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
