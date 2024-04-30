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
    Scene m_scene;
    Vec2 m_viewport_size = {0.0f, 0.0f};
    Vec2 m_quad_pos = {0.0f, 0.0f};
    ARef<Karen::FrameBuffer> m_frame_buff;
  };
}
