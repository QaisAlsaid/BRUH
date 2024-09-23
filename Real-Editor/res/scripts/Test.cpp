#include "Test.h"
#include "Karen/Core/ButtonsAndKeyCodes.h"
#include "Karen/Core/Input.h"
#include "Karen/Core/Math/math.h"
#include "Karen/Karen.h"
#include "Karen/Scene/Components.h"
#include <iostream>


#define KAREN_INFO(x) std::cout<<x<<std::endl;

extern "C" Karen::ScriptEntity* createScript()
{
  return static_cast<Karen::ScriptEntity*>(new TestScript());
}

extern "C" void destroyScript(Karen::ScriptEntity* instance)
{
  delete instance;
}

extern "C" void scriptInit(Karen::App* app)
{
  KAREN_CORE_ERROR("Created Script");
  std::cin.get();
}

void TestScript::onCreate()
{
  KAREN_INFO("Created dll Script!");
  m_entity.addComponent<Karen::SpriteComponent>();
}

Karen::Entity TestScript::getEntity()
{
  return m_entity;
}

void TestScript::onUpdate(Karen::Timestep ts)
{
  //m_entity.addComponent<Karen::SpriteComponent>().color = Karen::Vec4(1, 1, 1, 1);
  //std::cout<<"app*: "<<Karen::App::get()<<std::endl;
  

  if(Karen::Input::isKeyPressed(Karen::Keyboard::S))
    std::cout<<"S"<<std::endl;
/*  auto& sc = m_entity.getComponent<Karen::SpriteComponent>().color;
  
  sc.r += tan(ts);
  sc.g += sin(ts);
  sc.b += cos(ts);

  KAREN_INFO("Done color");

  auto* tc = m_entity.tryGetComponent<Karen::TransformComponent>();
  if(tc)
  {
    auto& pos = tc->position;
    if(Karen::Input::isKeyPressed(Karen::Keyboard::W))
      pos.y += 2 * ts;
    if(Karen::Input::isKeyPressed(Karen::Keyboard::S))
      pos.y -= 2 * ts;
    if(Karen::Input::isKeyPressed(Karen::Keyboard::D))
      pos.x += 2 * ts;
    if(Karen::Input::isKeyPressed(Karen::Keyboard::A))
      pos.x -= 2 * ts;
  }
  KAREN_INFO("Exited update");*/
}

void TestScript::onDestroy()
{
  KAREN_INFO("Destroyd dll Script");
  
  KAREN_INFO("Exited destroy");
}

