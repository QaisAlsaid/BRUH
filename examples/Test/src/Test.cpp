#include "Karen/ButtonsAndKeyCodes.h"
#include "Karen/Core/Timestep.h"
#include <Karen.h>

class mLayer : public Karen::Layer
{
public:
  mLayer() : Layer("Test") {}
  void onAttach() override 
  {
    m_r = new Karen::Renderer();
    KAREN_INFO("Layer: {0} Attached", this->getName());
    m_sh.reset(Karen::Shader::create());
    m_varr.reset(Karen::VertexArray::create());
    m_varr->bind();
    m_vbuf.reset(Karen::VertexBuffer::create(sizeof(float) * 9, m_verts, 5));
    m_ibuf.reset(Karen::IndexBuffer::create(3, m_inds, 5));
    
    m_sh->loadFromFile("../res/shaders/vs.glsl", "../res/shaders/fs.glsl");
    Karen::BufferLayout bl = 
    {
      {"pos", Karen::ShaderDataType::Float3}
    };
    m_vbuf->setLayout(bl);
    m_varr->setIndexBuffer(m_ibuf);
    m_varr->addVertexBuffer(m_vbuf);
  }
  void onDetach() override
  {
    KAREN_INFO("Layer: {0} Detached", this->getName());
    delete m_r;
  }

  void onUpdate(Karen::Timestep ts) override 
  {
    KAREN_INFO("delta sec: {0}, delta mil {1}", ts.getTime(), ts.getTimeAsMilliSeconds());
    if(Karen::Input::isKeyPressed(Karen::Keyboard::W))
      m_ortho.setPosition(Karen::Vec3(m_ortho.getPosition().x, m_ortho.getPosition().y + 0.005f, 0.0f));
    if(Karen::Input::isKeyPressed(Karen::Keyboard::S))
      m_ortho.setPosition(Karen::Vec3(m_ortho.getPosition().x, m_ortho.getPosition().y - 0.005f, 0.0f));
    if(Karen::Input::isKeyPressed(Karen::Keyboard::D))
      m_ortho.setPosition(Karen::Vec3(m_ortho.getPosition().x + 0.005f, m_ortho.getPosition().y, 0.0f));
   if(Karen::Input::isKeyPressed(Karen::Keyboard::A))
      m_ortho.setPosition(Karen::Vec3(m_ortho.getPosition().x - 0.005f, m_ortho.getPosition().y, 0.0f));
   if(Karen::Input::isKeyPressed(Karen::Keyboard::Up))
     m_ortho.setZoom(m_ortho.getZoom() + 0.02f);
   if(Karen::Input::isKeyPressed(Karen::Keyboard::Down))
     m_ortho.setZoom(m_ortho.getZoom() - 0.02f);
   if(Karen::Input::isKeyPressed(Karen::Keyboard::R))
     m_ortho.setZoom(1.0f);
   if(Karen::Input::isKeyPressed(Karen::Keyboard::Right))
     m_ortho.setRotation(m_ortho.getRotation() + 0.5f);
   if(Karen::Input::isKeyPressed(Karen::Keyboard::Left))
     m_ortho.setRotation(m_ortho.getRotation() -0.5f);
   if(Karen::Input::isKeyPressed(Karen::Keyboard::Z))
     m_ortho.setRotation(0.0f);
   m_ortho.onUpdate(3.0f);
  Karen::RenderCommands::clear(Karen::Vec4(0.2f, 0.2f, 0.2f, 1.0f));
    m_r->beginScene(m_ortho);
    m_r->submit(m_varr, m_sh);
    m_r->endScene();
  if(Karen::Input::isMouseButtonPressed(Karen::Mouse::ButtonLeft)) KAREN_CORE_TRACE("BU");
  }

  void onEvent(Karen::Event& e) override
  {
    if(e.getEventType() == Karen::EventType::MouseButtonPressed) KAREN_CORE_TRACE("BE");
  }

private:
  Karen::Renderer* m_r;
  Karen::ARef<Karen::Shader> m_sh;
  Karen::ARef<Karen::VertexArray> m_varr;
  Karen::ARef<Karen::VertexBuffer> m_vbuf;
  Karen::ARef<Karen::IndexBuffer> m_ibuf;
  Karen::OrthographicCamera m_ortho = Karen::OrthographicCamera(-1.0f, 1.0f, -1.0f, 1.0f);
  float m_verts[9] = 
  {
    -0.5f, -0.5f, 0.0f,
     0.5f,  0.0f, 0.0f,
     0.0f,  0.5f, 0.0f
  };

  uint32_t m_inds[3] = 
  {
    0, 1, 2
  };
};
class Sandbox : public Karen::App
{
public:
  Sandbox()
  {
    pushLayer(new mLayer());
  }
  ~Sandbox()
  {

  }
};

Karen::App* Karen::createApp()
{
  KAREN_SET_LOGLEVEL(Karen::Log::LogLevel::Trace);
  return new Sandbox();
}
