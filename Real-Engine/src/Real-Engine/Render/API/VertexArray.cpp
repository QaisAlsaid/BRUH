#include "pch.h"
#include "Real-Engine/Render/API/VertexArray.h"
#include "Platforms/OpenGl/OpenGlVertexArray.h"
#include "Real-Engine/Render/API/RendererAPI.h"
namespace Real
{
  ARef<VertexArray> VertexArray::create()
  {
    switch(RendererAPI::getAPI())
    {
      case RendererAPI::API::NONE:
        REAL_CORE_ASSERT_MSG(false, "Render API None not supported at the moment");
      case RendererAPI::API::OpenGl:
        return Real::createARef<OpenGlVertexArray>();
    }
    REAL_CORE_ASSERT_MSG(false, "Unknown API");
    return nullptr;
  }
}
