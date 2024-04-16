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
  Karen::ARef<Karen::Texture2D> m_tux;
  Karen::OrthographicCameraController m_ortho;
};
