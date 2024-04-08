#include "pch.h"
#include "Karen/Render/API/Renderer.h"
#include "Karen/Render/API/RenderCommands.h"


namespace Karen
{
  void Renderer::init()
  {
    RenderCommands::init();
  }

  void Renderer::beginScene(const OrthographicCamera& m_camera)
  {
    m_scene_data.proj_view = m_camera.getProjView();
  }

  void Renderer::endScene()
  {

  }

  void Renderer::submit(const ARef<VertexArray>& p_varr, const ARef<Shader>& p_shader, const Mat4& p_transform)
  {
    p_shader->bind();
    p_shader->setUniform("u_pv", m_scene_data.proj_view);
    p_shader->setUniform("u_trans", p_transform);
    RenderCommands::drawIndexed(p_varr);
  }
}
