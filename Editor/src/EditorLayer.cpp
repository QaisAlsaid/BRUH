#include "Karen/Render/API/RendererCapabilities.h"
#include "pch.h"
#include <Karen/Karen.h>
#include "EditorLayer.h"
#include <imgui.h>


namespace Karen
{

  EditorLayer::EditorLayer()
    :Layer(),/* m_ortho(0.0f, 100.0f, 0.0f, 100.0f)//*/m_ortho(Karen::Vec3(50.0f, 50.0f, 0.0f), Karen::Vec2(100.0f, 100.0f))//m_ortho(Karen::OrthographicCameraController(1280.0f/720.0f, Karen::Vec4(0.0f, 100.0f, 0.0f, 100.0f)))
  {
    KAREN_START_INSTRUMENTOR();
    RenderCommands::init();
  }

  void EditorLayer::onAttach()
  {
    activate();
    Renderer2D::init("../res/shaders/Shaders2D/config.xml");
    KAREN_INFO("Layer: {0} Attached", name);
    m_quad_pos = Vec2(50.0f);
    m_ortho.setSpeed({100.0f, 100.0f});
    m_ortho.getCamera().setZoomLimits(0.01, 50.0f);
    FrameBuffer::Specs s;
    s.width = 1280;
    s.height = 720;
    s.is_swap_chain_target = true;
    m_frame_buff = FrameBuffer::create(s);
    KAREN_CORE_SET_LOGLEVEL(Log::LogLevel::Warn);
  
    m_ortho.getCamera().setZoom(m_ortho.getCamera().getZoom() + 20.0f);
  }

  int i=100;
  float t_s = 0.0f;
  void EditorLayer::onUpdate(Timestep ts)
  {
    t_s = ts;
    if(i-- < 0) KAREN_STOP_INSTRUMENTOR();
    KAREN_PROFILE_FUNCTION();
    {
      KAREN_PROFILE_SCOPE("Camera_Update");
      m_ortho.onUpdate(ts); 
    }
    if(Input::isKeyPressed(Karen::Keyboard::Up))
      m_quad_pos.y += 50.0f * ts;
    if(Input::isKeyPressed(Karen::Keyboard::Down))
      m_quad_pos.y -= 50.0f * ts;
    if(Input::isKeyPressed(Karen::Keyboard::Right))
      m_quad_pos.x += 50.0f * ts;
    if(Input::isKeyPressed(Karen::Keyboard::Left))
      m_quad_pos.x -= 50.0f * ts;
    if(Input::isKeyPressed(Karen::Keyboard::I))
      m_ortho.zoom(2.0f * ts);
    if(Input::isKeyPressed(Karen::Keyboard::K))
      m_ortho.zoom(-2.0f * ts);
  
    //Render   
    {
      
      KAREN_PROFILE_SCOPE("Render");
  
      m_frame_buff->bind();
  
      Renderer2D::resetStats();
      Renderer2D::clear(Karen::Vec4(0.24f, 0.24f, 0.24f, 1.0f));
      Renderer2D::beginScene(m_ortho.getCamera());
      Renderer2D::endScene();
      m_frame_buff->unbind();
    }
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
      m_ortho.onViewportResize(k_panel_size.x, k_panel_size.y);
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

  }

}
