#include "Karen/Core/Assertion.h"
#include "Karen/Core/Utils/FileDialogs.h"
#include "Karen/Scene/SceneSerializer.h"
#include "pch.h"
#include <Karen/Karen.h>
#include "EditorLayer.h"
#include <imgui.h>
#include "EditorSerializer.h"

namespace Karen
{

  EditorLayer::EditorLayer()
    : Layer("EditorLayer"), m_content_browser("../res"), m_asset_manager_panel(&App::get()->assetManager())
  {
    activate();

    KAREN_START_INSTRUMENTOR();
    
    RenderCommands::init();
    Renderer2D::init("../res/shaders/Shaders2D/config.xml");
  }

  void EditorLayer::onAttach()
  {
    m_scene = createARef<Scene>();
    m_helper_windows["Stats"] = createScoped<StatsWindow>();
  
          
    m_default_font_size = 18;
    m_default_font = "../res/fonts/Roboto/Roboto-Regular.ttf";
    m_imgui_ini_path = ".";
    

    setColorScheme(); 
    deSerializeEditor("../res/config/test.xml");
    if(m_default_editor)
      setColorScheme(); 
    initImGui();

    FrameBuffer::Specs s;
    s.width = 1280;
    s.height = 720;
    s.is_swap_chain_target = true;
    m_frame_buff = FrameBuffer::create(s);
    KAREN_CORE_SET_LOGLEVEL(Log::LogLevel::Warn);
    
    m_scene_hierarchy_panel.setContext(m_scene);
  }


  void EditorLayer::onUpdate(Timestep ts)
  {
    m_time_step = ts;
    Renderer2D::resetStats();
    m_frame_buff->bind();
    Renderer2D::clear(Vec4(0.25f, 0.25f, 0.25f, 1.0f));
    switch(m_scene_state)
    {
      case SceneState::Play:
      {
        m_scene->onUpdate(ts);
        break;
      }
      case SceneState::Stop:
      {
        m_scene->onEditorUpdate(ts);
        break;
      }
    }
      m_frame_buff->unbind();
  }

  void EditorLayer::onGuiUpdate()
  {
    static bool* p_open = new bool;
    *p_open = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

    if (opt_fullscreen)
    {
      const ImGuiViewport* viewport = ImGui::GetMainViewport();

      ImGui::SetNextWindowPos(viewport->WorkPos);
      ImGui::SetNextWindowSize(viewport->WorkSize);
      ImGui::SetNextWindowViewport(viewport->ID);

      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

      window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
      dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
      window_flags |= ImGuiWindowFlags_NoBackground;

    if (!opt_padding)
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec2 min_win_size = style.WindowMinSize;
    style.WindowMinSize = ImVec2(230, 100);//TODO: make it changeable from Editor Settings
    ImGui::Begin("DockSpace", p_open, window_flags);

    if (!opt_padding)
      ImGui::PopStyleVar();

    if (opt_fullscreen)
      ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);


    ImGui::End();
    style.WindowMinSize = min_win_size;
    
    const Stats stats = {Renderer2D::getStats(), m_time_step};
    ((StatsWindow*)m_helper_windows["Stats"].get())->stats = stats;
    
    updateMenuBar();
    updatePanels();

    ImGui::Begin("__DEBUG__");
    const char * label = m_scene_state == SceneState::Stop ? "Play" : "Stop";
    if(ImGui::Button(label))
    {
      switch(m_scene_state)
      {
        case SceneState::Play:
          onSceneStop();
          break;
        case SceneState::Stop:
          onScenePlay();
          break;
      }
    }
    ImGui::End();


    auto& io = ImGui::GetIO();
    if(io.WantSaveIniSettings)
      ImGui::SaveIniSettingsToDisk(m_imgui_ini_path.c_str());
  }

  void EditorLayer::onScenePlay()
  {
    m_scene_state = SceneState::Play;
    m_scene->onStart();
  }

  void EditorLayer::onSceneStop()
  {
    m_scene_state = SceneState::Stop;
    m_scene->onEnd();
  }

  void EditorLayer::onEvent(Event& e)
  {
  }

  void EditorLayer::onDetach()
  {
    auto& io = ImGui::GetIO();
    if(io.WantSaveIniSettings)
      ImGui::SaveIniSettingsToDisk(m_imgui_ini_path.c_str());
    
    m_scene->onEnd();
    serializeEditor("../res/config/test.xml");
  }

  void EditorLayer::updatePanels()
  {
    m_asset_manager_panel.onImGuiUpdate();
    m_content_browser.onImGuiUpdate();
    m_helper_windows["Stats"]->onImGuiUpdate();
    m_scene_hierarchy_panel.onGuiUpdate();
    auto e = m_scene_hierarchy_panel.getCurrentSelected();
    m_inspector_panel.setCurrentSelected(e);
    m_inspector_panel.onGuiUpdate();

    ImGui::ShowDemoWindow(&m_show_imgui_demo);

    ImGui::Begin("Viewport");
    const ImVec2 panel_size = ImGui::GetContentRegionAvail();
    Vec2 k_panel_size = {panel_size.x, panel_size.y};
    if(m_viewport_size != k_panel_size)
    {
      m_frame_buff->reSize(k_panel_size.x, k_panel_size.y);
      m_viewport_size = k_panel_size;
      m_scene->onViewportResize(m_viewport_size.x, m_viewport_size.y);
    }
    ImGui::Image((void*)(uintptr_t)m_frame_buff->getColorAttachmentId(), panel_size, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();

  }

  void EditorLayer::updateMenuBar()
  {
    if(ImGui::BeginMainMenuBar())
    {
      if(ImGui::BeginMenu("File"))
      {
        if(ImGui::MenuItem("New")) 
        {
          m_scene_hierarchy_panel.clearSelection();
          m_scene = createARef<Scene>();
          m_scene_hierarchy_panel.setContext(m_scene);
          m_inspector_panel.setCurrentSelected({});
        }
  
        ImGui::Separator();
        if(ImGui::MenuItem("Open", "Ctrl+O")) 
        {
          const auto& path = FileDialogs::OpenFile("yaml", "Karen Scene (.yaml)");
          KAREN_TRACE("path: {0}", path);
          if(!path.empty())
          {
            SceneSerializer ss(m_scene);
            if(!ss.deSerializeText(path.c_str()))
            {
              KAREN_CORE_ERROR("Error in deSerializeText(): Scene: {0}", path);
              m_scene = createARef<Scene>("Scene");
            }
            m_scene_hierarchy_panel.setContext(m_scene);
            m_scene_hierarchy_panel.clearSelection();
            m_inspector_panel.setCurrentSelected({});
          }
          else KAREN_TRACE("Cancele");
        }

        ImGui::Separator();
  
        if(ImGui::MenuItem("Save As..")) 
        {
          const auto& path = FileDialogs::SaveFile("yaml", "Karen Scene (.Karen)");
          KAREN_TRACE("path: {0}", path);
          if(!path.empty())
          {
            SceneSerializer ss(m_scene);
            ss.serializeText(path.c_str());
          }
        }

        ImGui::EndMenu();
      }
      if(ImGui::BeginMenu("Editor"))
      {
        ImGui::Separator();
        const auto& stats_window = m_helper_windows["Stats"];
        ImGui::Checkbox("Show Stats Window", &stats_window->is_active);
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
  }

  void EditorLayer::serializeEditor(const char* path)
  {
    EditorSerializer es(this);
    if(!es.serialize(path))
      KAREN_ASSERT(false);
  }

  void EditorLayer::deSerializeEditor(const char* path)
  {
    EditorSerializer es(this);
    if(!es.deSerialize(path))
      m_default_editor = true;
  }


  void EditorLayer::setColorScheme()
  {
    m_colors["WindowBg"] = Vec4(0.1f, 0.1f, 0.1f, 1.0f);
    m_colors["HeaderHovered"] = Vec4(0.2f, 0.2f, 0.2f, 1.0f);
    m_colors["HeaderActive"] = Vec4(0.17f, 0.17f, 0.17f, 1.0f);
    m_colors["ChildBg"] = Vec4(0.1f, 0.1f, 0.12f, 1.0f); 
    m_colors["PopupBg"] = Vec4(0.32f, 0.2f, 0.2f, 1.0f);
    m_colors["Border"] = Vec4(0.45f, 0.45f, 0.54f, 1.0f);
    m_colors["BorderShadow"] = Vec4(0.1f, 0.1f, 0.1f, 0.7f);
    m_colors["MenuBarBg"] = Vec4(0.1f, 0.1f, 0.1f, 1.0f);
    m_colors["ScrollbarBg"] = Vec4(0.3f, 0.3f ,0.32f, 1.0f);
    m_colors["ScrollbarGrab"] = Vec4(0.4f, 0.4f, 0.43f, 1.0f);
    m_colors["ScrollbarGrabHovered"] = Vec4(0.5f, 0.5f, 0.5f, 1.0f);
    m_colors["ScrollbarGrabActive"] = Vec4(0.4f, 0.4f, 0.6f, 1.0f);
    m_colors["CheckMark"] = Vec4(0.4f, 0.47f, 0.4f, 1.0f);
    m_colors["SliderGrab"] = Vec4(0.3f, 0.3f, 0.3f, 1.0f);
    m_colors["SliderGrabActive"] = Vec4(0.3f, 0.3f, 0.4f, 1.0f);
    m_colors["Header"] = Vec4(0.32f, 0.32f, 0.39f, 1.0f);
    m_colors["Separator"] = Vec4(0.6f, 0.6f, 0.6f, 0.8f);
    m_colors["SeparatorHovered"] = Vec4(0.3f, 0.3f, 0.3f, 0.9f);
    m_colors["SeparatorActive"] = Vec4(0.3f, 0.43f, 0.3f ,1.0f);
    m_colors["ResizeGrip"] = Vec4(0.3f, 0.3f, 0.5f, 0.6f);
    m_colors["ResizeGripHovered"] = Vec4(0.3f, 0.3f, 0.6f, 0.7f);
    m_colors["ResizeGripActive"] = Vec4(0.3f, 0.3f, 0.7f, 0.8f);
    m_colors["DockingPreview"] = Vec4(0.32f, 0.32f, 0.32f, 1.0f);
    m_colors["DockingEmptyBg"] = Vec4(0.32f, 0.32f, 0.32f, 1.0f);
    m_colors["PlotLines"] = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
    m_colors["PlotLinesHovered"] = Vec4(1.0f);
    m_colors["PlotHistogram"] = Vec4(1.0f);
    m_colors["PlotHistogramHovered"] = Vec4(1.0f);
    m_colors["TableHeaderBg"] = Vec4(1.0f);
    m_colors["TableBorderStrong"] = Vec4(1.0f);
    m_colors["TableBorderLight"] = Vec4(1.0f);
    m_colors["TableRowBg"] = Vec4(1.0f);
    m_colors["TableRowBgAlt"] = Vec4(1.0f);
    m_colors["TextSelectedBg"] = Vec4(1.0f, 1.0f, 1.0f, 0.4f);
    m_colors["DragDropTarget"] = Vec4(0.2f, 0.45f, 0.35f, 1.0f);
    m_colors["NavHighlight"] = Vec4(1.0f);
    m_colors["NavWindowingHighlight"] = Vec4(1.0f);
    m_colors["NavWindowingDimBg"] = Vec4(0.2f, 0.2f, 0.2f , 0.4f);
    m_colors["ModalWindowDimBg"] = Vec4(0.2f, 0.2f, 0.2f, 0.4f);
    m_colors["Button"] = Vec4(0.2f, 0.2f, 0.2f, 1.0f);
    m_colors["ButtonHovered"] = Vec4(0.3f, 0.3f, 0.3f, 1.0f);
    m_colors["ButtonActive"] = Vec4(0.15f, 0.15f, 0.15f, 1.0f);
    m_colors["FrameBg"] = Vec4(0.2f, 0.2f, 0.2f, 1.0f);
    m_colors["FrameBgHovered"] = Vec4(0.3f, 0.3f, 0.3f, 1.0f);
    m_colors["FrameBgActive"] = Vec4(0.17f, 0.1f, 0.19f, 1.0f);
    m_colors["Tab"] = Vec4(0.14f, 0.17f, 0.13f, 1.0f);
    m_colors["TabHovered"] = Vec4(0.3f, 0.34f, 0.35f, 1.0f);
    m_colors["TabActive"] = Vec4(0.1f, 0.25f, 0.25f, 1.0f);
    m_colors["TabUnfocused"] = Vec4(0.07f, 0.2f, 0.13f, 1.0f);
    m_colors["TabUnfocusedActive"] = Vec4(0.21f, 0.23f, 0.22f, 1.0f);
    m_colors["TitleBg"] = Vec4(0.15f, 0.154f, 0.145f, 1.0f);
    m_colors["TitleBgActive"] = Vec4(0.2f, 0.25f, 0.3f, 1.0f);
    m_colors["TitleBgCollapsed"] = Vec4(0.2f, 0.3f, 0.6f, 1.0f); 
    
  }

  void EditorLayer::initImGui()
  {
    //Ini
    ImGui::LoadIniSettingsFromDisk(m_imgui_ini_path.c_str());
    
    //Font
    auto& io = ImGui::GetIO();
    io.FontDefault = io.Fonts->AddFontFromFileTTF(m_default_font.c_str(), m_default_font_size);
    
    //Colors
    auto& colors = ImGui::GetStyle().Colors;
  
    colors[ImGuiCol_WindowBg] = *(ImVec4*)&m_colors.at("WindowBg");
    colors[ImGuiCol_HeaderHovered] = *(ImVec4*)&m_colors.at("HeaderHovered");
    colors[ImGuiCol_HeaderActive] = *(ImVec4*)&m_colors.at("HeaderActive");
    colors[ImGuiCol_Button] = *(ImVec4*)&m_colors.at("Button");
    colors[ImGuiCol_ButtonHovered] = *(ImVec4*)&m_colors.at("ButtonHovered");
    colors[ImGuiCol_ButtonActive] = *(ImVec4*)&m_colors.at("ButtonActive");
    colors[ImGuiCol_FrameBg] = *(ImVec4*)&m_colors.at("FrameBg");
    colors[ImGuiCol_FrameBgHovered] = *(ImVec4*)&m_colors.at("FrameBgHovered");
    colors[ImGuiCol_FrameBgActive] = *(ImVec4*)&m_colors.at("FrameBgActive");
    colors[ImGuiCol_Tab] = *(ImVec4*)&m_colors.at("Tab");
    colors[ImGuiCol_TabHovered] = *(ImVec4*)&m_colors.at("TabHovered");
    colors[ImGuiCol_TabActive] = *(ImVec4*)&m_colors.at("TabActive");
    colors[ImGuiCol_TabUnfocused] = *(ImVec4*)&m_colors.at("TabUnfocused");
    colors[ImGuiCol_TabUnfocusedActive] = *(ImVec4*)&m_colors.at("TabUnfocusedActive");
    colors[ImGuiCol_TitleBg] = *(ImVec4*)&m_colors.at("TitleBg");
    colors[ImGuiCol_TitleBgActive] = *(ImVec4*)&m_colors.at("TitleBgActive");
    colors[ImGuiCol_TitleBgCollapsed] = *(ImVec4*)&m_colors.at("TitleBgCollapsed");
    colors[ImGuiCol_ChildBg] = *(ImVec4*)&m_colors.at("ChildBg"); 
    colors[ImGuiCol_PopupBg] = *(ImVec4*)&m_colors.at("PopupBg");
    colors[ImGuiCol_Border] = *(ImVec4*)&m_colors.at("Border");
    colors[ImGuiCol_BorderShadow] = *(ImVec4*)&m_colors.at("BorderShadow");
    colors[ImGuiCol_MenuBarBg] = *(ImVec4*)&m_colors.at("MenuBarBg");
    colors[ImGuiCol_ScrollbarBg] = *(ImVec4*)&m_colors.at("ScrollbarBg");
    colors[ImGuiCol_ScrollbarBg] = *(ImVec4*)&m_colors.at("ScrollbarGrab");
    colors[ImGuiCol_ScrollbarBg] = *(ImVec4*)&m_colors.at("ScrollbarGrabHovered");
    colors[ImGuiCol_ScrollbarGrabActive] =  *(ImVec4*)&m_colors.at("ScrollbarGrabActive");
    colors[ImGuiCol_CheckMark] =  *(ImVec4*)&m_colors.at("CheckMark");
    colors[ImGuiCol_SliderGrab] =  *(ImVec4*)&m_colors.at("SliderGrab");
    colors[ImGuiCol_SliderGrabActive] =  *(ImVec4*)&m_colors.at("SliderGrabActive");
    colors[ImGuiCol_Header] =  *(ImVec4*)&m_colors["Header"];
    colors[ImGuiCol_Separator] =  *(ImVec4*)&m_colors["Separator"];
    colors[ImGuiCol_SeparatorHovered] =  *(ImVec4*)&m_colors.at("SeparatorHovered");
    colors[ImGuiCol_SeparatorActive] =  *(ImVec4*)&m_colors.at("SeparatorActive");
    colors[ImGuiCol_ResizeGrip] =  *(ImVec4*)&m_colors.at("ResizeGrip");
    colors[ImGuiCol_ResizeGripHovered] =  *(ImVec4*)&m_colors.at("ResizeGripHovered");
    colors[ImGuiCol_ResizeGripActive] =  *(ImVec4*)&m_colors.at("ResizeGripActive");
    colors[ImGuiCol_DockingPreview] =  *(ImVec4*)&m_colors.at("DockingPreview");
    colors[ImGuiCol_DockingEmptyBg] =  *(ImVec4*)&m_colors.at("DockingEmptyBg");
    colors[ImGuiCol_PlotLines] =  *(ImVec4*)&m_colors.at("PlotLines");
    colors[ImGuiCol_PlotLinesHovered] =  *(ImVec4*)&m_colors.at("PlotLinesHovered");
    colors[ImGuiCol_PlotHistogram] =  *(ImVec4*)&m_colors.at("PlotHistogram");
    colors[ImGuiCol_PlotHistogramHovered] =  *(ImVec4*)&m_colors.at("PlotHistogramHovered");
    colors[ImGuiCol_TableHeaderBg] =  *(ImVec4*)&m_colors.at("TableHeaderBg");
    colors[ImGuiCol_TableBorderStrong] =  *(ImVec4*)&m_colors.at("TableBorderStrong");
    colors[ImGuiCol_TableBorderLight] =  *(ImVec4*)&m_colors.at("TableBorderLight");
    colors[ImGuiCol_TableRowBg] =  *(ImVec4*)&m_colors.at("TableRowBg");
    colors[ImGuiCol_TableRowBgAlt] =  *(ImVec4*)&m_colors.at("TableRowBgAlt");
    colors[ImGuiCol_TextSelectedBg] =  *(ImVec4*)&m_colors.at("TextSelectedBg");
    colors[ImGuiCol_DragDropTarget] =  *(ImVec4*)&m_colors.at("DragDropTarget");
    colors[ImGuiCol_NavHighlight] =  *(ImVec4*)&m_colors.at("NavHighlight");
    colors[ImGuiCol_NavWindowingHighlight] =  *(ImVec4*)&m_colors.at("NavWindowingHighlight");
    colors[ImGuiCol_NavWindowingDimBg] =  *(ImVec4*)&m_colors.at("NavWindowingDimBg");
    colors[ImGuiCol_ModalWindowDimBg] = *(ImVec4*)&m_colors.at("ModalWindowDimBg");

  }
}

