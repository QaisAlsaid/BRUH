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
  Karen::ARef<Karen::Texture2D> m_tux;
  Karen::OrthographicCameraController m_ortho = /*Karen::OrthographicCameraController((float)1280/(float)720);*/Karen::OrthographicCameraController(0.0f, 1280.0f, 0.0f, 720.0f);
  Karen::Vec3 m_rect_pos;
  Karen::Vec2 m_tux_pos;
  float m_rect_speed = 200.0f; 
  Karen::Vec2 m_rect_scale;
  Karen::Vec2 m_tux_scale;
};
