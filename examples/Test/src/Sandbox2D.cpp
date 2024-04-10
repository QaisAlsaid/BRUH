#include <Karen/Karen.h>
#include "Karen/Render/Renderer2D/Renderer2D.h"
#include "Sandbox2D.h"

Sandbox2DLayer::Sandbox2DLayer()
  :Karen::Layer("Sandbox2D")
{
  activate();
}

void Sandbox2DLayer::onAttach()
{
  Karen::Renderer2D::init("../res/shaders/Shaders2D/config.xml");
  m_tux = Karen::Texture2D::create("../res/textuers/img1.png");
  m_rect_pos = Karen::Vec3(0.0f, 0.0f, 0.0f);
  m_tux_pos = Karen::Vec2(0.0f, 0.0f);
  m_tux_scale = Karen::Vec2(m_tux->getWidth(), m_tux->getHeight());
  m_rect_scale = Karen::Vec2(100.0f);
  KAREN_INFO("Layer: {0} Attached", name);
}

void Sandbox2DLayer::onUpdate(Karen::Timestep ts)
{
  m_ortho.onUpdate(ts); 
  if(Karen::Input::isKeyPressed(Karen::Keyboard::Right))
    m_rect_pos.x += m_rect_speed * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::Left))
    m_rect_pos.x -= m_rect_speed * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::Up))
    m_rect_pos.y += m_rect_speed * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::Down))
    m_rect_pos.y -= m_rect_speed * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::NumPadAdd))
    m_rect_scale += Karen::Vec2(1.0f) * ts.getTime();
  if(Karen::Input::isKeyPressed(Karen::Keyboard::Minus))
    m_rect_scale -= Karen::Vec2(1.0f) * ts.getTime();
  if(Karen::Input::isKeyPressed(Karen::Keyboard::V))
    this->visible = this->visible == true ? false : true;

}

void Sandbox2DLayer::onRender()
{
  Karen::Renderer2D::clear(Karen::Vec4(0.24f, 0.24f, 0.24f, 1.0f));
  Karen::Renderer2D::beginScene(m_ortho.getCamera());
  Karen::Renderer2D::drawQuad(Karen::Vec3(m_rect_pos.x + m_rect_scale.x/2.0f,m_rect_pos.y + m_rect_scale.y/2.0f, 0.0f) , Karen::Vec2(m_rect_scale), 0.0f, Karen::Vec4(0.8f, 0.3f, 0.3f, 1.0f));
  Karen::Renderer2D::drawQuad(Karen::Vec2(m_tux_pos.x + m_tux_scale.x/2.0f,m_tux_pos.y + m_tux_scale.y/2.0f) , Karen::Vec2(m_tux_scale), 0.0f, m_tux);
  Karen::Renderer2D::endScene();
}

void Sandbox2DLayer::onEvent(Karen::Event& e)
{

}

void Sandbox2DLayer::onDetach()
{

}
