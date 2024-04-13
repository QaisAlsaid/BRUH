#include <Karen/Karen.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include "Karen/Core/Events/AppEvents.h"
#include "Karen/Core/Log.h"
#include "Karen/Physics/Physics2D/Collisions.h"
#include "Karen/Physics/Physics2D/RigidBody.h"
#include "Karen/Render/API/RenderCommands.h"
#include "Karen/Render/Renderer2D/Renderer2D.h"
#include "Sandbox2D.h"


Sandbox2DLayer::Sandbox2DLayer()
  :Karen::Layer("Sandbox2D")
{
  Karen::RenderCommands::init();
}

void Sandbox2DLayer::onAttach()
{ 
  srand(time(0));
  //activate();
  Karen::Renderer2D::init("../res/shaders/Shaders2D/config.xml");
  m_tux = Karen::Texture2D::create("../res/textuers/img1.png");
  /*for(int i = 0; i < 5; ++i)
  {
    Karen::ARef<Karen::RigidBody2D> b;
    if(!(i%2))
      b = Karen::RigidBody2D::createRectangle(Karen::ShapeProps(), Karen::Vec2(0.2f), Karen::Vec2((float)(rand()%7)), 0.0f);
    else
      b = Karen::RigidBody2D::createCircle(Karen::ShapeProps(), 1.0f/m_tux->getWidth(), Karen::Vec2((float)1/float(rand()%7)), 0.0f);
    m_bodys.push_back(b);
  }*/
  m_bodys.push_back(Karen::RigidBody2D::createRectangle(Karen::ShapeProps(), Karen::Vec2(1.0f), Karen::Vec2(), 0.0f));
  KAREN_INFO("Layer: {0} Attached", name);
  m_rect_pos = Karen::Vec2(0.0f);
  m_circle_pos = Karen::Vec2(0.0f);
}

void Sandbox2DLayer::onUpdate(Karen::Timestep ts)
{
  Karen::Vec2 m_rect_speed(100.0f, 100.5f);
  m_ortho.onUpdate(ts); 
  if(Karen::Input::isKeyPressed(Karen::Keyboard::Right))
    m_rect_pos.x += m_rect_speed.x * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::Left))
    m_rect_pos.x -= m_rect_speed.x * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::Up))
    m_rect_pos.y += m_rect_speed.y * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::Down))
    m_rect_pos.y -= m_rect_speed.y * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::V))
    this->visible = this->visible == true ? false : true;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::L))
    m_circle_pos.x += m_rect_speed.y * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::J))
    m_circle_pos.x -= m_rect_speed.y * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::I))
    m_circle_pos.y += m_rect_speed.x * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::K))
    m_circle_pos.y -= m_rect_speed.x * ts;
    
  m_bodys.at(0)->setPosition(m_rect_pos);
  //m_bodys.at(3)->setPosition(m_circle_pos);
  /*for(uint32_t i = 0; i < m_bodys.size() - 1; ++i)
  {
    auto& b1 = m_bodys.at(i);
    b1->setRotation(b1->getRotation() + 90.0f * ts.getTime());
    for(uint32_t j = i+1; j < m_bodys.size(); ++j)
    {
      auto& b2 = m_bodys.at(j);
      b2->setRotation(b2->getRotation() + 90.0f * ts.getTime());
      if(b1->m_props.type == Karen::ShapeProps::ShapeType::Rectangle && b2->m_props.type == Karen::ShapeProps::ShapeType::Rectangle)
      { 
        const auto& sta1 = Karen::Collision::intersect(b1->getTransformedVertices(), b2->getTransformedVertices());
        if(sta1.is_collision)
        {
          KAREN_WARN("tow rects {0}, {1}", i, j);
          KAREN_WARN("rect1_pos: {0}, rect2_pos: {1}", b1->getPosition(), b2->getPosition());
          b1->setPosition(b1->getPosition() + (sta1.normal * sta1.depth/2.0f));
          b2->setPosition(b2->getPosition() + (-sta1.normal * sta1.depth/2.0f));
        }
      }
      else if(b1->m_props.type == Karen::ShapeProps::ShapeType::Circle && b2->m_props.type == Karen::ShapeProps::ShapeType::Circle)
      {
        const auto& sta2 = Karen::Collision::intersect(b1->getPosition(), b1->getRadius()/2.0f, b2->getPosition(), b2->getRadius()/2.0f);
        if(sta2.is_collision)
        {
          KAREN_WARN("two circles {0}, {1}", i, j);
          KAREN_WARN("circle_pos: {0}, circle_pos: {1}", b1->getPosition(), b2->getPosition());
          
          b1->setPosition(b1->getPosition() + (sta2.normal * sta2.depth/2.0f));
          b2->setPosition(b2->getPosition() + (-sta2.normal * sta2.depth/2.0f));
        }
      }
    }
  }*/
}

void Sandbox2DLayer::onRender()
{
  Karen::Renderer2D::clear(Karen::Vec4(0.24f, 0.24f, 0.24f, 1.0f));
  Karen::Renderer2D::beginScene(m_ortho.getCamera());
  Karen::Vec4 color;
  for(int i = 0; i < m_bodys.size(); ++i)
  {
    const auto& b = m_bodys.at(i);
    if(m_bodys.at(i)->m_props.type == Karen::ShapeProps::ShapeType::Circle){
    Karen::Renderer2D::drawTexturedQuad(b->getPosition(), Karen::Vec2(b->getRadius()), b->getRotation(), m_tux);
    KAREN_WARN("Called for circle pos: {0}, size: {1}", b->getPosition(), Karen::Vec2(b->getRadius()));
    }else 
    Karen::Renderer2D::drawQuad(b->getPosition(), b->getSize(), b->getRotation(), Karen::Vec4(0.9, 0.25, 0.2, 1.0f));

  }
  Karen::Renderer2D::endScene();
}


void Sandbox2DLayer::onEvent(Karen::Event& e)
{
  m_ortho.onEvent(e);
}

void Sandbox2DLayer::onDetach()
{

}
