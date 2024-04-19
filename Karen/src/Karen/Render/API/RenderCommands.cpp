#include "pch.h"
#include "Karen/Render/API/RendererAPI.h"
#include "Karen/Render/API/RenderCommands.h"
#include "Platforms/OpenGl/OpenGlRendererAPI.h"


namespace Karen
{
  RendererAPI* RenderCommands::s_API = new OpenGLRendererAPI; 
}
