#include "pch.h"
#include "Render/API/Renderer.h"
#include "Render/API/RenderCommands.h"


namespace Karen
{
  void Renderer::beginScene(const OrthographicCamera& m_camera)
  {
    m_scene_data.proj_view = m_camera.getProjView();
  }

  void Renderer::endScene()
  {

  }

  void Renderer::submit(const ARef<VertexArray>& p_varr, const ARef<Shader>& p_shader)
  {
    p_shader->bind();
    p_shader->setUniformMat4fv("u_pv", m_scene_data.proj_view);
    RenderCommands::drawIndexed(p_varr);
  }
}
