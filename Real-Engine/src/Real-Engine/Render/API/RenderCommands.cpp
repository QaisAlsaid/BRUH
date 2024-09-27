#include "pch.h"
#include "Real-Engine/Render/API/RendererAPI.h"
#include "Real-Engine/Render/API/RenderCommands.h"
#include "Platforms/OpenGl/OpenGlRendererAPI.h"


namespace Real
{
  RendererAPI* RenderCommands::s_API = new OpenGLRendererAPI; 
}
