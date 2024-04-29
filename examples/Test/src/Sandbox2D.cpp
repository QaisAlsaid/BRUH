#include "Karen/Scene/Components.h"
#include "pch.h"
#include <Karen/Karen.h>
#include "Sandbox2D.h"
#include <imgui.h>
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
  auto e = m_scene.addEntity("colored quad");
  e.addComponent<Karen::SpriteComponent>(Karen::Vec4(0.8f, 0.25f, 0.8f, 1.0f));
  e.getComponent<Karen::TransformComponent>().position = {10.0f, 10.0f, 1.0f};
  auto ec = m_scene.addEntity("camera");
  ec.addComponent<Karen::CameraComponent>(glm::ortho(0.0f, 100.0f, 0.0f, 100.0f));
  ec.getComponent<Karen::CameraComponent>().is_primary = true;
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
    m_quad_pos.y += 5.0f * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::Down))
    m_quad_pos.y -= 5.0f * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::Right))
    m_quad_pos.x += 5.0f * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::Left))
    m_quad_pos.x -= 5.0f * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::I))
    m_ortho.zoom(2.0f * ts);
  if(Karen::Input::isKeyPressed(Karen::Keyboard::K))
    m_ortho.zoom(-2.0f * ts);
  
  //Render 
  {
    KAREN_PROFILE_SCOPE("Render");
    Karen::Renderer2D::resetStats();
    Karen::Renderer2D::clear(Karen::Vec4(0.24f, 0.24f, 0.24f, 1.0f));
    //Karen::Renderer2D::beginScene(m_ortho.getCamera());
    m_scene.onUpdate(ts);
    //Karen::Renderer2D::endScene();
  }
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
