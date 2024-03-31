#include <Karen.h>

class mLayer : public Karen::Layer
{
public:
  mLayer() : Layer("Test") {}
  void onAttach()        override 
  {
    m_r = new Karen::Renderer();
    KAREN_INFO("Layer: {0} Attached", this->getName());
    m_sh.reset(Karen::Shader::create());
    m_varr.reset(Karen::VertexArray::create());
    m_varr->bind();
    m_vbuf.reset(Karen::VertexBuffer::create(sizeof(float) * 9, m_verts, 5));
    m_ibuf.reset(Karen::IndexBuffer::create(3, m_inds, 5));

    Karen::BufferLayout bl = 
    {
      {"pos", Karen::ShaderDataType::Float3}
    };
    m_vbuf->setLayout(bl);
    m_varr->setIndexBuffer(m_ibuf);
    m_varr->addVertexBuffer(m_vbuf);
  }
  void onDetach()       override 
  {
    KAREN_INFO("Layer: {0} Detached", this->getName());
    delete m_r;
  }

  void onUpdate()        override 
  {
    if(Karen::Input::isKeyPressed(Karen::Keyboard::Q)) KAREN_TRACE("Q");
    Karen::RenderCommands::clear(Karen::Vec4(1.0f));
    m_r->beginScene();
    m_r->submit(m_varr);
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
