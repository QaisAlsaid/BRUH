#include "Karen/Render/API/RendererCapabilities.h"
#include "pch.h"
#include <Karen/Karen.h>
#include "Sandbox2D.h"
#include "../../../Karen/vendor/imgui/imgui.h"
Sandbox2DLayer::Sandbox2DLayer()
  :Karen::Layer("Sandbox2D"),/* m_ortho(0.0f, 100.0f, 0.0f, 100.0f)//*/m_ortho(Karen::Vec3(50.0f, 50.0f, 0.0f), Karen::Vec2(100.0f, 100.0f))//m_ortho(Karen::OrthographicCameraController(1280.0f/720.0f, Karen::Vec4(0.0f, 100.0f, 0.0f, 100.0f)))
{
  KAREN_START_INSTRUMENTOR();
  Karen::RenderCommands::init();
}

void Sandbox2DLayer::onAttach()
{
  activate();
  Karen::Renderer2D::init("../res/shaders/Shaders2D/config.xml");
  KAREN_INFO("Layer: {0} Attached", name);
  m_quad_pos = Karen::Vec2(50.0f);
  m_ortho.setSpeed({100.0f, 100.0f});
  m_ortho.getCamera().setZoomLimits(0.01, 50.0f);
  Karen::FrameBuffer::Specs s;
  s.width = 1280;
  s.height = 720;
  s.is_swap_chain_target = true;
  m_frame_buff = Karen::FrameBuffer::create(s);
  KAREN_CORE_SET_LOGLEVEL(Karen::Log::LogLevel::Warn);
}

int i=100;
float t_s = 0.0f;
void Sandbox2DLayer::onUpdate(Karen::Timestep ts)
{
  t_s = ts;
  if(i-- < 0) KAREN_STOP_INSTRUMENTOR();
  KAREN_PROFILE_FUNCTION();
  {
    KAREN_PROFILE_SCOPE("Camera_Update");
    m_ortho.onUpdate(ts); 
  }
  if(Karen::Input::isKeyPressed(Karen::Keyboard::Up))
    m_quad_pos.y += 50.0f * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::Down))
    m_quad_pos.y -= 50.0f * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::Right))
    m_quad_pos.x += 50.0f * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::Left))
    m_quad_pos.x -= 50.0f * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::I))
    m_ortho.zoom(2.0f * ts);
  if(Karen::Input::isKeyPressed(Karen::Keyboard::K))
    m_ortho.zoom(-2.0f * ts);
  if(Karen::Input::isKeyPressed(Karen::Keyboard::V))
    this->visible = this->visible == true ? false : true;
}

void Sandbox2DLayer::onRender()
{
  KAREN_PROFILE_FUNCTION();
  KAREN_PROFILE_SCOPE("Render");
  //m_frame_buff->bind();
  Karen::Renderer2D::resetStats();
  Karen::Renderer2D::clear(Karen::Vec4(0.24f, 0.24f, 0.24f, 1.0f));
  Karen::Renderer2D::beginScene(m_ortho.getCamera());
  //Karen::Renderer2D::drawQuad(Karen::Vec3(m_quad_pos, 0.0f), {25.0f, 25.0f}, 0.0f, m_tux);
  //Karen::Renderer2D::drawQuad(Karen::Vec3(75.0f, 0.0f, 0.0f), {25.0f, 25.0f}, 45.0f, Karen::Vec4(0.3f, 0.6f, 0.5f, 1.0f));
  //Karen::Renderer2D::drawQuad(Karen::Vec2(70.0f, 50.0f), {25.0f, 25.0f}, 0.0f, m_tux);
  for(int i = 0; i < 100; ++i)
    for(int j = 0; j < 100; ++j)
    {
      Karen::Renderer2D::drawQuad(Karen::Vec2(11.0f * i, 11.0f * j), {10.0f, 10.0f}, Karen::Vec4(0.3f, 1.0f/float(i * j) / t_s, 1.0f * (float)j/2.0f * (float)i * t_s, 1.0f));
    }

  Karen::Renderer2D::endScene();
  //m_frame_buff->unbind();
}

void Sandbox2DLayer::onGuiUpdate()
{
  const auto stats = Karen::Renderer2D::getStats();
  ImGui::Begin("Stats");
  ImGui::Text("App::Stats Timestep: %f", t_s);
  ImGui::Text("Renderer2D::Stats: Draw Calls: %d", stats.draw_calls);
  ImGui::Text("Renderer2D::Stats: Quad Count: %d", stats.quad_count);
  ImGui::Text("Renderer2D::Stats: Index Count: %d", stats.getIndexCount());
  ImGui::Text("Renderer2D::Stats: Vertex Count: %d", stats.getVertexCount());
  //ImGui::Image((void*)m_frame_buff->getColorAttachmentId(), ImVec2(1280, 720), ImVec2(0, 1), ImVec2(1, 0));
  ImGui::End();
}

void Sandbox2DLayer::onEvent(Karen::Event& e)
{
  m_ortho.onEvent(e);
}

void Sandbox2DLayer::onDetach()
{

}
