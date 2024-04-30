#include "pch.h"
#include <Karen/Karen.h>
#include "EditorLayer.h"
#include <imgui.h>
class Script : public Karen::ScriptEntity
{
public:
  void onCreate() override {KAREN_WARN("created Script");}
  void onUpdate(Karen::Timestep ts) override {
    tt += ts;
    tc += ts;
    if(tc > 1.0f) tc = 0.0001f;
    if(t)
    {
      KAREN_WARN("{0}", ts.getTime());
      t=false;
    }
    auto& trans = m_entity.getComponent<Karen::TransformComponent>();
    trans.scale = Karen::Vec2(sin(tt) * 10.0f);
    trans.position = Karen::Vec3(std::abs(cos(tt)) * 100.0f, std::abs(sin(tt)) * 100.0f, 0.0f);
    auto& sprite = m_entity.getComponent<Karen::SpriteComponent>();
    sprite.color = Karen::Vec4(cos(tc), 1/tc, sin(tc), 1.0f);
  }
  void onDestroy() override {KAREN_WARN("destroy entity");}
private: 
  bool t = true;
  double tt = 0.0f, tc = 0.0001f;
};

namespace Karen
{

  EditorLayer::EditorLayer()
    : Layer("EditorLayer")
  {
    KAREN_START_INSTRUMENTOR();
    RenderCommands::init();
  }

  void EditorLayer::onAttach()
  {
    activate();
    Renderer2D::init("../res/shaders/Shaders2D/config.xml");
    FrameBuffer::Specs s;
    s.width = 1280;
    s.height = 720;
    s.is_swap_chain_target = true;
    m_frame_buff = FrameBuffer::create(s);
    KAREN_CORE_SET_LOGLEVEL(Log::LogLevel::Warn);
    auto e = m_scene.addEntity("Quad"); 
    e.addComponent<SpriteComponent>().color = Vec4(0.7f, 0.25f, 0.7f, 1.0f);
    auto& ct = e.getComponent<TransformComponent>();
    ct.position = Vec3(99.0f, 99.0f, 0.0f);
    ct.scale = Vec2(1.0f, 1.0f);
    auto ce = m_scene.addEntity("Camera");
    auto& cc = ce.addComponent<CameraComponent>();
    cc.camera.setOrtho({100.0f, 100.0f});
    cc.is_primary = true;
    auto& nsc = e.addComponent<NativeScriptComponent>();
    nsc.bind<Script>();
    m_scene.onStart();
  }

  int i=100;
  float t_s = 0.0f;
  void EditorLayer::onUpdate(Timestep ts)
  {
    t_s = ts;
    Renderer2D::resetStats();
    m_frame_buff->bind();
    Renderer2D::clear(Vec4(0.25f, 0.25f, 0.25f, 1.0f));
    m_scene.onUpdate(ts);
    m_frame_buff->unbind();
  }

  void EditorLayer::onGuiUpdate()
  {
    static bool* p_open = new bool;
    *p_open = true;
    static bool opt_fullscreen = true; // Is the Dockspace full-screen?
    static bool opt_padding = false; // Is there padding (a blank space) between the window edge and the Dockspace?
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None; // Config flags for the Dockspace

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    // Is the example in Fullscreen mode?
    if (opt_fullscreen)
    {
        // If so, get the main viewport:
      const ImGuiViewport* viewport = ImGui::GetMainViewport();

        // Set the parent window's position, size, and viewport to match that of the main viewport. This is so the parent window
        // completely covers the main viewport, giving it a "full-screen" feel.
      ImGui::SetNextWindowPos(viewport->WorkPos);
      ImGui::SetNextWindowSize(viewport->WorkSize);
      ImGui::SetNextWindowViewport(viewport->ID);

        // Set the parent window's styles to match that of the main viewport:
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); // No corner rounding on the window
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // No border around the window

        // Manipulate the window flags to make it inaccessible to the user (no titlebar, resize/move, or navigation)
      window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        // The example is not in Fullscreen mode (the parent window can be dragged around and resized), disable the
        // ImGuiDockNodeFlags_PassthruCentralNode flag.
      dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so the parent window should not have its own background:
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
      window_flags |= ImGuiWindowFlags_NoBackground;

    // If the padding option is disabled, set the parent window's padding size to 0 to effectively hide said padding.
    if (!opt_padding)
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::Begin("DockSpace", p_open, window_flags);

    // Remove the padding configuration - we pushed it, now we pop it:
    if (!opt_padding)
      ImGui::PopStyleVar();

    // Pop the two style rules set in Fullscreen mode - the corner rounding and the border size.
    if (opt_fullscreen)
      ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);


    // This is to show the menu bar that will change the config settings at runtime.
    // If you copied this demo function into your own code and removed ImGuiWindowFlags_MenuBar at the top of the function,

 


    // End the parent window that contains the Dockspace:
    ImGui::End();

    const auto stats = Renderer2D::getStats();

    ImGui::Begin("Stats");
    ImGui::Text("App::Stats Timestep: %f", t_s);
    ImGui::Text("Renderer2D::Stats: Draw Calls: %d", stats.draw_calls);
    ImGui::Text("Renderer2D::Stats: Quad Count: %d", stats.quad_count);
    ImGui::Text("Renderer2D::Stats: Index Count: %d", stats.getIndexCount());
    ImGui::Text("Renderer2D::Stats: Vertex Count: %d", stats.getVertexCount());
    ImGui::End();
  
    ImGui::Begin("ViewPort");
    const ImVec2 panel_size = ImGui::GetContentRegionAvail();
    Vec2 k_panel_size = {panel_size.x, panel_size.y};
    if(m_viewport_size != k_panel_size)
    {
      m_frame_buff->reSize(k_panel_size.x, k_panel_size.y);
      m_viewport_size = k_panel_size;
      m_scene.onViewportResize(m_viewport_size.x, m_viewport_size.y);
    }
    ImGui::Image((void*)(uintptr_t)m_frame_buff->getColorAttachmentId(), panel_size, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();

  }


  void EditorLayer::onEvent(Event& e)
  {
    //m_ortho.onEvent(e);
  }

  void EditorLayer::onDetach()
  {
    m_scene.onEnd();
  }

}
