#include "pch.h"
#include "RendererCapabilities.h"
#include "RendererAPI.h"
#include "Platforms/OpenGl/OpenGlRendererCapabilities.h"


namespace Karen
{
  ARef<RendererCapabilities> RendererCapabilities::create()
  {
    switch(RendererAPI::getAPI())
    {
      case RendererAPI::API::OpenGl:
      {
        return Karen::createARef<OpenGlRendererCapabilities>();
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
