#include "pch.h"
#include "Karen/Render/API/RendererAPI.h"
#include "Karen/Render/API/RenderCommands.h"
#include "Platforms/OpenGl/OpenGlRendererAPI.h"


namespace Karen
{
  RendererAPI* RenderCommands::s_API = new OpenGLRendererAPI; 
  
  //TODO: Make it happen :) idk how
 /* switch(RendererAPI::getAPI())
  {
    case RendererAPI::API::NONE:
      KAREN_CORE_ASSERT("RendererAPI: NONE is Not supported");
    case RendererAPI::API::OpenGl:
      s_API = new OpenGlRendererAPI;
  }*/
}
