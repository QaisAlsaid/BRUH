#include <Karen/Karen.h>


class Sandbox2DLayer : public Karen::Layer
{
public: 
  Sandbox2DLayer();
  
  void onAttach() override;

  void onDetach() override;

  void onUpdate(Karen::Timestep ts) override;
  
  void onRender() override;

  void onEvent(Karen::Event& e) override;

 private:
  bool onResize(Karen::WindowResizeEvent& e);
  Karen::Vec2 m_rect_pos, m_circle_pos;
  std::vector<Karen::ARef<Karen::RigidBody2D>> m_bodys;
  Karen::ARef<Karen::Texture2D> m_tux;
  Karen::OrthographicCameraController m_ortho = Karen::OrthographicCameraController(0.0f, 1280.0f, 0.0f, 720.0f);
};
