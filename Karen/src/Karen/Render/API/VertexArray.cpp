#include "pch.h"
#include "Karen/Render/API/VertexArray.h"
#include "Platforms/OpenGl/OpenGlVertexArray.h"
#include "Karen/Render/API/RendererAPI.h"
namespace Karen
{
  VertexArray* VertexArray::create()
  {
    switch(RendererAPI::getAPI())
    {
      case RendererAPI::API::NONE:
        KAREN_CORE_ASSERT(false, "Render API None not supported at the moment");
      case RendererAPI::API::OpenGl:
        return new OpenGlVertexArray;
    }
    KAREN_CORE_ASSERT(false, "Unknown API");
    return nullptr;
  }
}
