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

  void onGuiUpdate() override;
 private:
  bool onResize(Karen::WindowResizeEvent& e);
  Karen::ARef<Karen::Texture2D> m_tux;
  Karen::OrthographicCameraController m_ortho;
  Karen::Vec2 m_quad_pos = {0.0f, 0.0f};
};
