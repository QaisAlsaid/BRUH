#include "pch.h"
#include "Render/API/Shader.h"
#include "Render/API/RendererAPI.h"
#include "Platforms/OpenGl/OpenGlShader.h"


namespace Karen
{
  Shader* create(const std::string& vp, const std::string& fp)
  {
    switch(RendererAPI::getAPI())
    {
      case RendererAPI::API::NONE:
        KAREN_CORE_ASSERT(false, "Renderer API: 'NONE' Not suported at the moment");
        return nullptr;
      case RendererAPI::API::OpenGl:
        return new OpenGlShader(vp, fp);
    }
    KAREN_CORE_ASSERT(false, "Unknown Renderer API");
    return nullptr;
  }
  
  Shader* Shader::create()
  {
    switch(RendererAPI::getAPI())
    {
      case RendererAPI::API::NONE:
        KAREN_CORE_ASSERT(false, "Render API: 'NONE' Not suported at the moment");
        return nullptr;
      case RendererAPI::API::OpenGl:
        return new OpenGlShader;
    }
    KAREN_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
  }
}
