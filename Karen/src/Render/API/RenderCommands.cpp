#include "pch.h"
#include "Render/API/RendererAPI.h"
#include "RenderCommands.h"
#include "Platforms/OpenGl/OpenGlRendererAPI.h"


namespace Karen
{
  RendererAPI* RenderCommands::s_API = new OpenGLRendererAPI;  
  
  void RenderCommands::clear(const Vec4& p_cc)
  {
    s_API->clear(p_cc);
  }

  void RenderCommands::drawIndexed(const ARef<VertexArray>& varr)
  {
    s_API->drawIndexed(varr);
  }

  //TODO: Make it happen :) idk how
 /* switch(RendererAPI::getAPI())
  {
    case RendererAPI::API::NONE:
      KAREN_CORE_ASSERT("RendererAPI: NONE is Not supported");
    case RendererAPI::API::OpenGl:
      s_API = new OpenGlRendererAPI;
  }*/
}
