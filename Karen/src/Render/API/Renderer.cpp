#include "pch.h"
#include "Render/API/Renderer.h"
#include "Render/API/RenderCommands.h"


namespace Karen
{
  void Renderer::beginScene()
  {

  }

  void Renderer::endScene()
  {

  }

  void Renderer::submit(const ARef<VertexArray>& p_varr)
  {
    RenderCommands::drawIndexed(p_varr);
  }
}
