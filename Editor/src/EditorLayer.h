#include <Karen/Karen.h>

namespace Karen
{
  class EditorLayer : public Karen::Layer
  {
  public: 
    EditorLayer();
  
    void onAttach() override;

    void onDetach() override;

    void onUpdate(Karen::Timestep ts) override;

    void onEvent(Karen::Event& e) override;

    void onGuiUpdate() override;
  private:
    Vec2 m_viewport_size = {0.0f, 0.0f};
    bool onResize(Karen::WindowResizeEvent& e);
    //Karen::ARef<Karen::Texture2D> m_tuxes[8];
    Karen::OrthographicCameraController m_ortho;
    Karen::Vec2 m_quad_pos = {0.0f, 0.0f};
    Karen::ARef<Karen::FrameBuffer> m_frame_buff;
  };
}
