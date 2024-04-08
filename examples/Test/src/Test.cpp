#include "Karen/Render/API/BufferLayout.h"
#include "Karen/Render/ShaderManager.h"
#include <Karen.h>

class mLayer : public Karen::Layer
{
public:
  mLayer() : Layer("Test") {}
  void onAttach() override 
  {
    m_r->init();
    m_tri_pos = Karen::Vec3(-0.5f, 0.5f, 0.0f);
    m_rect_pos = Karen::Vec3(-0.5f, 0.5f, 0.0f);
    m_r.reset(new Karen::Renderer());
    KAREN_INFO("Layer: {0} Attached", this->getName());
    m_color_sh = Karen::Shader::create();
    m_texture_sh = Karen::Shader::create();
    m_tri_varr.reset(Karen::VertexArray::create());
    m_tri_varr->bind();
    m_tri_vbuf.reset(Karen::VertexBuffer::create(sizeof(float) * 9, m_tri_verts, 5));
    m_tri_ibuf.reset(Karen::IndexBuffer::create(3, m_tri_inds, 5));
   
    m_rect_varr.reset(Karen::VertexArray::create());
    m_rect_vbuf.reset(Karen::VertexBuffer::create(sizeof(float) * 20, m_rect_verts, 5));
    m_rect_ibuf.reset(Karen::IndexBuffer::create(6, m_rect_inds, 5));
#ifdef KAREN_EMSCRIPTEN
    m_color_sh->loadFromFile("res/shaders/color_vs_gles.glsl", "res/shaders/color_fs_gles.glsl");
    m_texture_sh->loadFromFile("res/shaders/tux_vs_gles.glsl", "res/shaders/tux_fs_gles.glsl");
    m_tux = Karen::Texture2D::create("res/textuers/img1.png");
#else 
    m_color_sh->loadFromFile("../res/shaders/color_vs.glsl", "../res/shaders/color_fs.glsl");
    m_texture_sh->loadFromFile("../res/shaders/tux_vs.glsl", "../res/shaders/tux_fs.glsl");
    m_tux = Karen::Texture2D::create("../res/textuers/img1.png");
#endif //KAREN_EMSCRIPTEN
    Karen::BufferLayout rect_bl = 
    {
      {"pos", Karen::ShaderDataType::Float3},
      {"tux_coord", Karen::ShaderDataType::Float2}
    };
    Karen::BufferLayout tri_bl = 
    {
      {"pos", Karen::ShaderDataType::Float3}
    };
    m_tri_vbuf->setLayout(tri_bl);
    m_tri_varr->setIndexBuffer(m_tri_ibuf);
    m_tri_varr->addVertexBuffer(m_tri_vbuf);
    
    m_rect_vbuf->setLayout(rect_bl);
    m_rect_varr->setIndexBuffer(m_rect_ibuf);
    m_rect_varr->addVertexBuffer(m_rect_vbuf);
    m_ortho.setZoom(1.0f);
    
    m_color_sh = m_shaders.get("color");
  }
  void onDetach() override
  {
    KAREN_INFO("Layer: {0} Detached", this->getName());
  }

  void onUpdate(Karen::Timestep ts) override 
  {
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
    tri_trans = glm::scale(tri_trans, Karen::Vec3(0.4f, 0.4f, 0.4f));
    
    Karen::Mat4 rect_trans = glm::translate(Karen::Mat4(1.0f), m_rect_pos);
    rect_trans = glm::scale(rect_trans, Karen::Vec3(1.0f, 1.0f, 1.0f));
    Karen::RenderCommands::clear(Karen::Vec4(0.8f, 0.2f, 0.2f, 1.0f));
    m_r->beginScene(m_ortho);
    m_tux->bind(0);
    m_texture_sh->bind();
    //m_texture_sh->setUniformInt("u_tux", 0);
    m_r->submit(m_rect_varr, m_texture_sh, rect_trans);
    m_r->submit(m_tri_varr, m_color_sh, tri_trans);
    m_r->endScene();
  }

  void onEvent(Karen::Event& e) override
  {
  }

private:
  Karen::ShaderManager m_shaders = Karen::ShaderManager("../res/shaders/config.xml");
  Karen::ARef<Karen::Renderer> m_r;
  Karen::ARef<Karen::Shader> m_color_sh;
  Karen::ARef<Karen::Shader> m_texture_sh;
  Karen::ARef<Karen::VertexArray> m_tri_varr;
  Karen::ARef<Karen::VertexBuffer> m_tri_vbuf;
  Karen::ARef<Karen::IndexBuffer> m_tri_ibuf;
  Karen::ARef<Karen::VertexBuffer> m_rect_vbuf;
  Karen::ARef<Karen::IndexBuffer> m_rect_ibuf;
  Karen::ARef<Karen::VertexArray> m_rect_varr;
  Karen::ARef<Karen::Texture2D> m_tux;
  Karen::OrthographicCamera m_ortho = Karen::OrthographicCamera(-1.0f, 1.0f, -1.0f, 1.0f);
  Karen::Vec3 m_tri_pos;
  Karen::Vec3 m_rect_pos;
  const float m_tri_speed  = 0.3f;
  const float m_rect_speed = 0.2f;
  const float m_tri_verts[9] = 
  {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f
  };

  const uint32_t m_tri_inds[3] = 
  {
    0, 1, 2
  };

  const float m_rect_verts[20]= 
  {
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
  };

  const uint32_t m_rect_inds[6] = 
  {
    0, 1, 2,
    2, 3, 0
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
