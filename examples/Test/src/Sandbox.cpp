#include "Sandbox.h"
#include "Karen/Render/API/Shader.h"

SandboxLayer::SandboxLayer()
  : Karen::Layer("Sandbox")
{
}

void SandboxLayer::onAttach() 
{
  //activate();
  m_ortho.getCamera().setZoomLimits(1.0f, 100.0f);
  m_r->init();
  m_rect_pos = Karen::Vec3(0.0f, 0.0f, 0.0f);
  m_tri_pos = Karen::Vec3(0.0f);
  m_r.reset(new Karen::Renderer());
  KAREN_INFO("Layer: {0} Attached", this->getName());
  m_tri_varr = (Karen::VertexArray::create());
  m_tri_varr->bind();
  m_tri_vbuf = (Karen::VertexBuffer::create(sizeof(float) * 15, m_tri_verts));
  m_tri_ibuf = (Karen::IndexBuffer::create(3, m_tri_inds, 5));
   
  m_rect_varr = (Karen::VertexArray::create());
  m_rect_vbuf = (Karen::VertexBuffer::create(sizeof(float) * 20, m_rect_verts));
  m_rect_ibuf = (Karen::IndexBuffer::create(6, m_rect_inds, 5));
#ifdef KAREN_EMSCRIPTEN
  m_color_sh->loadFromFile("res/shaders/color_vs_gles.glsl", "res/shaders/color_fs_gles.glsl");
  m_texture_sh->loadFromFile("res/shaders/tux_vs_gles.glsl", "res/shaders/tux_fs_gles.glsl");
  m_tux = Karen::Texture2D::create("res/textuers/img1.png");
#else 
  m_tux = Karen::Texture2D::create("../res/textuers/img1.png");
#endif //KAREN_EMSCRIPTEN
  Karen::BufferLayout rect_bl = 
  {
    {"pos", Karen::ShaderDataType::Float3},
    {"tux_coord", Karen::ShaderDataType::Float2}
  };
  Karen::BufferLayout tri_bl = 
  {
    {"pos", Karen::ShaderDataType::Float3},
    {"normals", Karen::ShaderDataType::Float2}
  };
  m_tri_vbuf->setLayout(tri_bl);
  m_tri_varr->setIndexBuffer(m_tri_ibuf);
  m_tri_varr->addVertexBuffer(m_tri_vbuf);
    
  m_rect_vbuf->setLayout(rect_bl);
  m_rect_varr->setIndexBuffer(m_rect_ibuf);
  m_rect_varr->addVertexBuffer(m_rect_vbuf); 
}

void SandboxLayer::onDetach()
{
  KAREN_INFO("Layer: {0} Detached", this->getName());
}

void SandboxLayer::onUpdate(Karen::Timestep ts)
{
  if(Karen::Input::isKeyPressed(Karen::Keyboard::I))
    m_tri_pos.y += m_tri_speed * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::K))
    m_tri_pos.y -= m_tri_speed * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::L))
    m_tri_pos.x += m_tri_speed * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::J))
    m_tri_pos.x -= m_tri_speed * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::W))
    m_rect_pos.y += m_tri_speed * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::S))
    m_rect_pos.y -= m_tri_speed * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::D))
    m_rect_pos.x += m_tri_speed * ts;
  if(Karen::Input::isKeyPressed(Karen::Keyboard::A))
    m_rect_pos.x -= m_tri_speed * ts;
 

  if(Karen::Input::isKeyPressed(Karen::Keyboard::V))
    this->visible = this->visible == true ? false : true;
   
  m_ortho.onUpdate(ts);
  
  Karen::Mat4 tri_trans = glm::translate(Karen::Mat4(1.0f), m_tri_pos);
  tri_trans = glm::scale(tri_trans, Karen::Vec3(0.4f, 0.4f, 0.4f));
    
  Karen::Mat4 rect_trans = glm::translate(Karen::Mat4(1.0f), m_rect_pos);
  rect_trans = glm::scale(rect_trans, Karen::Vec3(1.0f, 1.0f, 1.0f));
  Karen::RenderCommands::clear(Karen::Vec4(0.8f, 0.2f, 0.2f, 1.0f));
    
  m_r->beginScene(m_ortho.getCamera());
  m_tux->bind(0);
  const auto& t_sh = m_shaders.get("tux_shader");
  const auto& c_sh = m_shaders.get("lightning_shader");
  m_r->submit(m_rect_varr, t_sh, rect_trans);
  c_sh->bind();
  c_sh->setUniform("u_light_pos", m_rect_pos);
  c_sh->setUniform("u_light_color", Karen::Vec4(1.0f));
  c_sh->setUniform("u_proj_view", m_ortho.getCamera().getProjView());
  c_sh->setUniform("u_trans", tri_trans);
  c_sh->setUniform("u_tint_color", Karen::Vec4(0.2f, 0.3f, 0.8f, 1.0f));
  Karen::RenderCommands::drawIndexed(m_tri_varr);
  m_r->endScene();
}

void SandboxLayer::onRender()
{
  
}
  void SandboxLayer::onEvent(Karen::Event& e)
  {
    if(e.getEventType() == Karen::EventType::WindowResized)
      KAREN_CORE_INFO("Event: {0} received [from OnEvent Test.cpp], Handled: [1]", e, e.isHandled());
    m_ortho.onEvent(e);
    //HACK: this should be handled in the camera controller but the renderer isn't static
    //so we will update the camera rect then we will set the viewport based on that
    if(e.getEventType() == Karen::EventType::WindowResized)
    {
      //const auto& r = m_ortho.getCamera().getRect();
      //m_r->onWindowResize(0, 0, r.z, r.w);
    }
  }

