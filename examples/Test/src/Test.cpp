#include <Karen.h>

class mLayer : public Karen::Layer
{
public:
  mLayer() : Layer("Test") {}
  void onAttach() override 
  {
    m_tri_pos = Karen::Vec3(-0.5f, 0.5f, 0.0f);
    m_rect_pos = Karen::Vec3(-0.5f, 0.5f, 0.0f);
    m_r = new Karen::Renderer();
    KAREN_INFO("Layer: {0} Attached", this->getName());
    m_sh.reset(Karen::Shader::create());
    m_tri_varr.reset(Karen::VertexArray::create());
    m_tri_varr->bind();
    m_tri_vbuf.reset(Karen::VertexBuffer::create(sizeof(float) * 9, m_tri_verts, 5));
    m_tri_ibuf.reset(Karen::IndexBuffer::create(3, m_tri_inds, 5));
   
    m_rect_varr.reset(Karen::VertexArray::create());
    m_rect_vbuf.reset(Karen::VertexBuffer::create(sizeof(float) * 12, m_rect_verts, 5));
    m_rect_ibuf.reset(Karen::IndexBuffer::create(6, m_rect_inds, 5));
#ifdef KAREN_EMSCRIPTEN
    m_sh->loadFromFile("vs_gles.glsl", "fs_gles.glsl");
#else 
    m_sh->loadFromFile("../res/shaders/vs.glsl", "../res/shaders/fs.glsl");
#endif //KAREN_EMSCRIPTEN
    Karen::BufferLayout bl = 
    {
      {"pos", Karen::ShaderDataType::Float3}
    };
    m_tri_vbuf->setLayout(bl);
    m_tri_varr->setIndexBuffer(m_tri_ibuf);
    m_tri_varr->addVertexBuffer(m_tri_vbuf);
    
    m_rect_vbuf->setLayout(bl);
    m_rect_varr->setIndexBuffer(m_rect_ibuf);
    m_rect_varr->addVertexBuffer(m_rect_vbuf);
    m_ortho.setZoom(1.0f);
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
      m_ortho.setPosition(Karen::Vec3(m_ortho.getPosition().x, m_ortho.getPosition().y + 0.5f * ts, 0.0f));
    if(Karen::Input::isKeyPressed(Karen::Keyboard::S))
      m_ortho.setPosition(Karen::Vec3(m_ortho.getPosition().x, m_ortho.getPosition().y - 0.5f * ts, 0.0f));
    if(Karen::Input::isKeyPressed(Karen::Keyboard::D))
      m_ortho.setPosition(Karen::Vec3(m_ortho.getPosition().x + 0.5f * ts, m_ortho.getPosition().y, 0.0f));
   if(Karen::Input::isKeyPressed(Karen::Keyboard::A))
      m_ortho.setPosition(Karen::Vec3(m_ortho.getPosition().x - 0.5f * ts, m_ortho.getPosition().y, 0.0f));
   if(Karen::Input::isKeyPressed(Karen::Keyboard::Up))
     m_ortho.setZoom(m_ortho.getZoom() + 0.2f * ts);
   if(Karen::Input::isKeyPressed(Karen::Keyboard::Down))
     m_ortho.setZoom(m_ortho.getZoom() - 0.2f * ts);
   if(Karen::Input::isKeyPressed(Karen::Keyboard::Z))
     m_ortho.setZoom(1.0f);
   if(Karen::Input::isKeyPressed(Karen::Keyboard::Right))
     m_ortho.setRotation(m_ortho.getRotation() + 180.0f * ts);
   if(Karen::Input::isKeyPressed(Karen::Keyboard::Left))
     m_ortho.setRotation(m_ortho.getRotation() -180.0f * ts);
   if(Karen::Input::isKeyPressed(Karen::Keyboard::R))
     m_ortho.setRotation(0.0f);
   if(Karen::Input::isKeyPressed(Karen::Keyboard::I))
     m_tri_pos.y += m_tri_speed * ts;
   if(Karen::Input::isKeyPressed(Karen::Keyboard::K))
     m_tri_pos.y -= m_tri_speed * ts;
   if(Karen::Input::isKeyPressed(Karen::Keyboard::L))
     m_tri_pos.x += m_tri_speed * ts;
   if(Karen::Input::isKeyPressed(Karen::Keyboard::J))
     m_tri_pos.x -= m_tri_speed * ts;

   m_ortho.onUpdate(ts);

    Karen::Mat4 tri_trans = glm::translate(Karen::Mat4(1.0f), m_tri_pos);
    tri_trans = glm::scale(tri_trans, Karen::Vec3(0.7f, 0.7f, 0.7f));

    Karen::Mat4 rect_trans = glm::translate(Karen::Mat4(1.0f), m_rect_pos);
    rect_trans = glm::scale(rect_trans, Karen::Vec3(0.4f, 0.4f, 0.4f));
    Karen::RenderCommands::clear(Karen::Vec4(0.8f, 0.2f, 0.2f, 1.0f));
    m_r->beginScene(m_ortho);
    m_r->submit(m_rect_varr, m_sh, rect_trans);
    m_r->submit(m_tri_varr, m_sh, tri_trans);
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
  Karen::ARef<Karen::VertexArray> m_tri_varr;
  Karen::ARef<Karen::VertexBuffer> m_tri_vbuf;
  Karen::ARef<Karen::IndexBuffer> m_tri_ibuf;
  Karen::ARef<Karen::VertexBuffer> m_rect_vbuf;
  Karen::ARef<Karen::IndexBuffer> m_rect_ibuf;
  Karen::ARef<Karen::VertexArray> m_rect_varr;
  Karen::OrthographicCamera m_ortho = Karen::OrthographicCamera(-1.0f, 1.0f, -1.0f, 1.0f);
  Karen::Vec3 m_tri_pos;
  Karen::Vec3 m_rect_pos;
  const float m_tri_speed  = 0.3f;
  const float m_rect_speed = 0.2f;
  const float m_tri_verts[9] = 
  {
    -0.5f, -0.5f, 0.0f,
     0.5f,  0.0f, 0.0f,
     0.0f,  0.5f, 0.0f
  };

  const uint32_t m_tri_inds[3] = 
  {
    0, 2, 1
  };

  const float m_rect_verts[12]= 
  {
    -0.5f, -0.5f, 0.0f,
     0.5f,  0.0f, 0.0f,
     0.0f,  0.5f, 0.0f,
     0.0f, -0.5f, 0.0f
  };

  const uint32_t m_rect_inds[6] = 
  {
    0, 1, 2,
    0, 1, 3
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
