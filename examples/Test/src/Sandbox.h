#include <Karen/Karen.h>

class SandboxLayer : public Karen::Layer
{
public:
  SandboxLayer();
  
  void onAttach() override;

  void onDetach() override;

  void onUpdate(Karen::Timestep ts) override;
  
  void onEvent(Karen::Event& e) override;

 private:
  Karen::ShaderManager m_shaders ; //Karen::ShaderManager("../res/shaders/config.xml");
  Karen::ARef<Karen::Renderer> m_r;
  Karen::ARef<Karen::VertexArray> m_tri_varr;
  Karen::ARef<Karen::VertexBuffer> m_tri_vbuf;
  Karen::ARef<Karen::IndexBuffer> m_tri_ibuf;
  Karen::ARef<Karen::VertexBuffer> m_rect_vbuf;
  Karen::ARef<Karen::IndexBuffer> m_rect_ibuf;
  Karen::ARef<Karen::VertexArray> m_rect_varr;
  Karen::ARef<Karen::Texture2D> m_tux;
  Karen::OrthographicCameraController m_ortho = Karen::OrthographicCameraController((1550.0f/2.0f)/720);
  Karen::Vec3 m_tri_pos;
  Karen::Vec3 m_rect_pos;
  const float m_tri_speed  = 0.3f;
  const float m_rect_speed = 0.2f;
  const float m_tri_verts[15] = 
  {
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
     0.0f,  0.5f, 0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, 0.0f, 0.0f, 1.0f
  };

  const uint32_t m_tri_inds[3] = 
  {
    0, 2, 1
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

