#include <Karen/Karen.h>
#include "Sandbox2D.h"
#include "Karen/Core/Instrumentor.h"

Sandbox2DLayer::Sandbox2DLayer()
  :Karen::Layer("Sandbox2D"), m_ortho(Karen::OrthographicCameraController(1280.0f/720.0f, Karen::Vec4(0.0f, 100.0f, 0.0f, 100.0f)))
{
  KAREN_START_INSTRUMENTOR();
  Karen::RenderCommands::init();
}

void Sandbox2DLayer::onAttach()
{
  activate();
  Karen::Renderer2D::init("../res/shaders/Shaders2D/config.xml");
  m_tux = Karen::Texture2D::create("../res/textuers/img1.png");
  KAREN_INFO("Layer: {0} Attached", name);
}

int i=100;
void Sandbox2DLayer::onUpdate(Karen::Timestep ts)
{
  if(i-- < 0) KAREN_STOP_INSTRUMENTOR();
  KAREN_PROFILE_FUNCTION();
  {
    KAREN_PROFILE_SCOPE("Camera_Update");
    m_ortho.onUpdate(ts); 
  }
  if(Karen::Input::isKeyPressed(Karen::Keyboard::V))
    this->visible = this->visible == true ? false : true;
}

void Sandbox2DLayer::onRender()
{
  KAREN_PROFILE_FUNCTION();
  KAREN_PROFILE_SCOPE("Render");
  Karen::Renderer2D::clear(Karen::Vec4(0.24f, 0.24f, 0.24f, 1.0f));
  Karen::Renderer2D::beginScene(m_ortho.getCamera());
  Karen::Renderer2D::drawQuad({0.f, 0.0f}, {100.0f, 100.0f}, 0.0f, Karen::Vec4(0.8, 0.25, 0.25, 1.0f));
  Karen::Renderer2D::endScene();
}


void Sandbox2DLayer::onEvent(Karen::Event& e)
{
  m_ortho.onEvent(e);
}

void Sandbox2DLayer::onDetach()
{

}
