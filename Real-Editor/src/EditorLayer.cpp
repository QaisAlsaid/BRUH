#include <pch.h>
#include "EditorLayer.h"
#include "EditorSerializer.h"
#include "Real-Engine/Core/App.h"
#include "Real-Engine/Core/ButtonsAndKeyCodes.h"
#include "Real-Engine/Core/Events/KeyEvents.h"
#include "Real-Engine/Core/Log.h"
#include "Real-Engine/Core/Timestep.h"
#include "Real-Engine/Scene/Components.h"
#include "Real-Engine/Scene/SceneSerializer.h"
#include "Real-Engine/Scene/ScriptEntity.h"
#include "glm/ext/quaternion_common.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Real-Engine/Scripting/Lua.h"
#include "glm/trigonometric.hpp"
#include <Real-Engine/Real-Engine.h>
#include <imgui.h>
#include <ImGuizmo.h>


#include "Real-Engine/Core/AssetManager.h"

//#ifdef REAL_PLATFORM_LINUX
//#include <dlfcn.h>
//#include "../res/scripts/Test.h"


//FIXME: change the native script API 'ASAP'

/*
void* handle;
Real-Engine::ScriptEntity* (*create)();
void (*destroy)(Real-Engine::ScriptEntity*);
void (*scriptInit)(Real-Engine::App*);
static Real-Engine::ScriptEntity* native = nullptr;

static Real-Engine::ScriptEntity* loadNativeScript(const char* path)
{
  handle = dlopen(path, RTLD_NOW);
  REAL_CORE_ASSERT(handle);
  create = (Real-Engine::ScriptEntity* (*)())dlsym(handle, "createScript");
  destroy = (void (*)(Real-Engine::ScriptEntity*))dlsym(handle, "destroyScript");
  scriptInit = (void (*)(Real-Engine::App*))dlsym(handle, "scriptInit");

  Real-Engine::ScriptEntity* myClass = (Karen::ScriptEntity*)create();
  REAL_INFO("Loaded");
  return myClass;
}
#endif*/
namespace Real
{
  /*class Scriptt : public ScriptEntity
  {
    public:
    float speed = 1;
    void onUpdate(Timestep ts) override
    {
      m_entity.getComponent<TransformComponent>().position.x += speed * ts;
    }
    void onDestroy() override{}
  };*/

  EditorLayer::EditorLayer()
    : Layer("EditorLayer"), m_content_browser("../res")
  {
    REAL_CORE_WARN("EditorLayer CTOR");
    bool x = AssetManager::loadConfig("../res/config/assets.test.xml");
    REAL_CORE_WARN("EXITED AssetManager::loadConfig WITH: {0}", x);
    activate();

    REAL_START_INSTRUMENTOR();
    
    RenderCommands::init();
    Renderer2D::init("../res/shaders/Shaders2D/config.xml");

/*#ifdef REAL_PLATFORM_LINUX
  native = loadNativeScript("../res/scripts/build/Script.so");
  scriptInit(Real-Engine::App::get());
#endif*/
  }


static float* speed = new float;
  void EditorLayer::onAttach()
  {
    REAL_CORE_WARN("CALLED ON ATTACH");
    auto uuid = AssetManager::getUUID("../res/config/scene.Real");
    if(uuid == UUID::invalid) REAL_CORE_ERROR("invalid main scene, UUID: {0}", uuid);
    m_scene_handle = uuid;
    m_editor_scene = AssetManager::get<AssetManager::SceneAsset>(uuid)->scene;

    
    m_scene = m_editor_scene;
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
    s.attachment_specs = {
      {FrameBuffer::TextureFormat::RGBA, FrameBuffer::TextureInternalFormat::RGBA8, "render_buffer"}, 
      {FrameBuffer::TextureFormat::RedInt, FrameBuffer::TextureInternalFormat::Int32, "id_buffer"},
      {FrameBuffer::TextureFormat::DepthStencil, FrameBuffer::TextureInternalFormat::Depth24Stencil8, "depth_buffer"}
    };
    s.width = 1280;
    s.height = 720;
    s.is_swap_chain_target = true;
    m_frame_buff = FrameBuffer::create(s);
    REAL_CORE_SET_LOGLEVEL(Log::LogLevel::Warn);

    m_scene_hierarchy_panel.setContext(m_scene);
    
//    nsc.bind<Scriptt>();

    
    /*auto nen = native->getEntity();
    auto endll = m_scene->copyEntity(nen);
    */
    //auto ee = m_scene->addEntity("");
    //auto& nscdll = ee.insertComponent<NativeScriptComponent>();
    //nscdll.bind(native);
   //TODO: Native script instance* is templated to the type given
    //speed = &((Script*)nsc.instance)->speed;
  
  }


  static bool first_time = true;
  
  void EditorLayer::onUpdate(Timestep ts)
  {
    if(m_scene_handle != UUID::invalid)
    {
      if(m_scene_state != SceneState::Play)
      {
        m_scene = AssetManager::get<AssetManager::SceneAsset>(m_scene_handle)->scene;
        m_editor_scene = m_scene;
      }
    }
    else REAL_CORE_ERROR("UUID::invalid Can't be used as Scene Handle");
    m_camera.onUpdate(ts);
    m_scene->setEditorCamera(m_camera.getView(), m_camera.getProjection());
    //TODO: callbacks for the context or something
    m_scene_hierarchy_panel.setContext(m_scene);
    if(Input::isKeyPressed(Keyboard::LeftControl) || Input::isKeyPressed(Keyboard::RightControl))
      handelCMD((int)Keyboard::LeftControl);
    m_time_step = ts;
    Renderer2D::resetStats();
    m_frame_buff->bind();
    //m_frame_buff->bindWriteFb(6);
    //Renderer2D::clear({200, 200, 200, 200});
    Renderer2D::clear(Vec4(0.25f, 0.25f, 0.25f, 1.0f));
/*
 #ifdef REAL_PLATFORM_LINUX
    //if(Input::isKeyPressed(Keyboard::Z))
    //{
      if(native)
      {
        native->onUpdate(ts);
        //std::cout<<"in Layer App*: "<<App::get()<<std::endl;
      }
    //}
    if(Input::isKeyPressed(Keyboard::P))
    {
      if(native)
        destroy(native);
      if(handle)
        dlclose(handle);
      native = loadNativeScript("../res/scripts/build/Script.so");
    }
#endif
    */
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
      auto abs_inv_mouse = ImGui::GetMousePos();
      Vec2 mouse(abs_inv_mouse.x, abs_inv_mouse.y);
      mouse -= m_min_vp_bounds;

      Vec2 vp_size = m_max_vp_bounds - m_min_vp_bounds;
      mouse.y = vp_size.y - mouse.y;
        

     // mouse = m_max_vp_bounds;
      //REAL_CORE_WARN("MOUSE: {0}", mouse);
      /*
      if(mouse.x > 0 && mouse.y > 0 && mouse.x < vp_size.x && mouse.y < vp_size.y)
      {
        int x = m_frame_buff->readPixelI(1, mouse.x, mouse.y);
        REAL_CORE_ERROR("ID: {0}, MOUSE: {1}", x, mouse);
      }
      auto se = m_scene_hierarchy_panel.getCurrentSelected(); 
      if(se && se.hasComponent<TransformComponent>())
      {
        m_scene->forEach<TransformComponent>([&](auto e, TransformComponent& tsc)
        {
          auto& pos = tsc.position;
          auto& scale = tsc.scale;
          //if(mouse.x > pos.x + scale.x/2.0f && mouse.x < pos.x - scale.x/2.0f &&
          //   mouse.y > pos.y + scale.y/2.0f && mouse.y < pos.y - scale.y/2.0f)
          REAL_CORE_ERROR("True entity id: {0}", (uint32_t)e);
        });
        auto val = m_frame_buff->readPixelI(1, mouse.x, mouse.y);
        REAL_ERROR("Value: {0}, mouse: {1}", val, mouse);
      }
      */
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
      ImGui::DragFloat("Cam run speed", speed);
    
//Editor Camera
    ImGui::Separator();
    ImGui::Text("Editor Camera");
    ImGui::DragFloat3("Position", glm::value_ptr(m_camera.position));
    auto yaw_deg = glm::degrees(m_camera.yaw), pitch_deg = glm::degrees(m_camera.pitch);
    ImGui::DragFloat("Yaw", &yaw_deg);
    m_camera.yaw = glm::radians(yaw_deg);
    ImGui::DragFloat("Pitch", &pitch_deg);
    m_camera.pitch = glm::radians(pitch_deg);
    auto fov_deg = glm::degrees(m_camera.fov);
    ImGui::DragFloat("Fov", &fov_deg, 0.5f, glm::degrees(m_camera.min_fov), glm::degrees(m_camera.max_fov));
    m_camera.fov = glm::radians(fov_deg);
    
    auto max_fov_deg = glm::degrees(m_camera.max_fov);
    ImGui::DragFloat("Max Fov", &max_fov_deg);
    m_camera.max_fov = glm::radians(max_fov_deg);

    auto min_fov_deg = glm::degrees(m_camera.min_fov);
    ImGui::DragFloat("Min Fov", &min_fov_deg);
    m_camera.min_fov = glm::radians(min_fov_deg);
    
    ImGui::DragFloat("Far", &m_camera.far);
    ImGui::DragFloat("Near", &m_camera.near);
    ImGui::DragFloat("Sensitivaty", &m_camera.mouse_sensitivaty);
    ImGui::Checkbox("F.A.R", &m_camera.fixed_aspect_ratio);
    ImGui::End();


    auto& io = ImGui::GetIO();
    if(io.WantSaveIniSettings)
      ImGui::SaveIniSettingsToDisk(m_imgui_ini_path.c_str());
  }

  void EditorLayer::changeScene(UUID id)
  {
    m_scene_handle = id;
  }

  void EditorLayer::onScenePlay()
  {
    m_scene_state = SceneState::Play;
    m_scene = Scene::copy(m_editor_scene);
    m_scene_hierarchy_panel.clearSelection();
    m_inspector_panel.setCurrentSelected( { } );
    App::get()->getExportedVariables().clear();
    m_scene->onStart();
  }

  void EditorLayer::onSceneStop()
  {
    m_scene_state = SceneState::Stop; 
    m_scene_hierarchy_panel.clearSelection();
    m_inspector_panel.setCurrentSelected( { } );
    m_scene->onEnd();
    m_scene = m_editor_scene;
  }

  void EditorLayer::onDetach()
  {
    auto& io = ImGui::GetIO();
    if(io.WantSaveIniSettings)
      ImGui::SaveIniSettingsToDisk(m_imgui_ini_path.c_str());
    
    serializeEditor("../res/config/test.xml");
  }

  void EditorLayer::updatePanels()
  {
    m_content_browser.onImGuiUpdate();
    m_helper_windows["Stats"]->onImGuiUpdate();
    m_scene_hierarchy_panel.onGuiUpdate();
    auto e = m_scene_hierarchy_panel.getCurrentSelected();
    m_inspector_panel.setCurrentSelected(e);
    m_inspector_panel.onGuiUpdate();

    ImGui::ShowDemoWindow(&m_show_imgui_demo);
    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoDecoration);
    auto vp_offset = ImGui::GetCursorPos();

    const ImVec2 panel_size = ImGui::GetContentRegionAvail();
    Vec2 k_panel_size = {panel_size.x, panel_size.y};
    if(m_viewport_size != k_panel_size)
    {
      m_frame_buff->reSize(k_panel_size.x, k_panel_size.y);
      m_viewport_size = k_panel_size;
      m_scene->onViewportResize(m_viewport_size.x, m_viewport_size.y);
      m_camera.onResize(m_viewport_size.x, m_viewport_size.y);
    }
    m_camera.aspect_ratio = m_viewport_size.x/m_viewport_size.y;
    ImGui::Image((void*)(uintptr_t)m_frame_buff->getColorAttachmentId("render_buffer"), panel_size, ImVec2(0, 1), ImVec2(1, 0));

    if(ImGui::BeginDragDropTarget())
    {
      if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_ASSET_HANDEL"))
      {
        UUID asset_handle = *(UUID*)payload->Data;
        REAL_CORE_WARN("Accepting drag and drop id: {0}", asset_handle);
        changeScene(asset_handle);
      }
      ImGui::EndDragDropTarget();
    }
    auto window_size = ImGui::GetWindowSize();
    auto min_vp_bounds = ImGui::GetWindowPos();

    m_min_vp_bounds = *(Vec2*)&min_vp_bounds;
    m_min_vp_bounds.x += vp_offset.x;
    m_min_vp_bounds.y += vp_offset.y;
    
    m_max_vp_bounds = m_min_vp_bounds;
    m_max_vp_bounds.x += window_size.x;
    m_max_vp_bounds.y += window_size.y;



    const ImVec2 win_pos = ImGui::GetWindowPos();

   /* auto mp = Real-Engine::Input::getMousePos();
    auto t_x = mp.x - win_pos.x;
    t_x = (t_x/window_size.x) * App::get()->getWindow().getWidth();

    auto t_y = mp.y - win_pos.y;
    t_y = App::get()->getWindow().getHeight() - (t_y / window_size.y * App::get()->getWindow().getHeight());

    m_max_vp_bounds.x = t_x;
    m_max_vp_bounds.y = t_y;
*/
    updateGizmos(*(Vec2*)&win_pos, *(Vec2*)&window_size);

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
          m_scene->clear();
          m_editor_scene->clear();
          m_editor_scene = createARef<Scene>();
          m_scene = m_editor_scene;
          m_scene_hierarchy_panel.setContext(m_scene);
          m_inspector_panel.setCurrentSelected({});
        }
  
        ImGui::Separator();
        if(ImGui::MenuItem("Open", "Ctrl+O")) 
        {
          //TODO : redirect to asset manager after open file
          const auto& path = FileDialogs::OpenFile("yaml", "Real-Engine Scene (.yaml)");
          REAL_TRACE("path: {0}", path);
          if(!path.empty())
          {
            SceneSerializer ss(m_scene);
            if(!ss.deSerializeText(path.c_str()))
            {
              REAL_CORE_ERROR("Error in deSerializeText(): Scene: {0}", path);
              m_scene = createARef<Scene>("Scene");
            }
            m_scene_hierarchy_panel.setContext(m_scene);
            m_scene_hierarchy_panel.clearSelection();
            m_inspector_panel.setCurrentSelected({});
          }
          else REAL_TRACE("Cancele");
        }

        ImGui::Separator();
  
        if(ImGui::MenuItem("Save As..")) 
        {
          const auto& path = FileDialogs::SaveFile("yaml", "Real-Engine Scene (.Karen)");
          REAL_TRACE("path: {0}", path);
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

  void EditorLayer::updateGizmos(Vec2 pos, Vec2 size)
  {
    Mat4 scene_cam_view, scene_cam_proj;
    static bool tf = false;
    static bool usc = false;
    ImGuizmo::SetOrthographic(tf);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);
    ImGui::Begin("__DEBUG__");
    ImGui::Checkbox("ortho", &tf);
    ImGui::Checkbox("use Scene Camera", &usc);
    ImGui::End();
    if(usc)
    {
      SceneCamera* camera = nullptr;
      TransformComponent* tc = nullptr;
      m_scene->forEach<CameraComponent>([&](auto e_id, CameraComponent& cc)
      {
        Entity e(e_id, m_scene.get());
        camera = cc.is_primary ? &cc.camera : nullptr;
        tc = e.tryGetComponent<TransformComponent>();
        REAL_CORE_ASSERT(tc);
      });
      if(camera && tc)
      {
        scene_cam_proj = camera->getProjection();
        scene_cam_view = glm::inverse(tc->getTransformationMatrix());
      }
    }
    auto current_selected = m_scene_hierarchy_panel.getCurrentSelected();
    if(current_selected)
    {
      auto* current_transform_component = current_selected.tryGetComponent<TransformComponent>();
      if(current_transform_component)
      {
        auto current_transformation = current_transform_component->getTransformationMatrix();
        const auto& cam_proj = usc ? scene_cam_proj : m_camera.getProjection();
        const auto& cam_view = usc ? scene_cam_view : m_camera.getView();
        ImGuizmo::Manipulate(glm::value_ptr(cam_view), glm::value_ptr(cam_proj),
          (ImGuizmo::OPERATION)m_op, ImGuizmo::LOCAL, glm::value_ptr(current_transformation));
        if(ImGuizmo::IsUsing())
        {
          Vec3 rotate;
          decompose(current_transformation, current_transform_component->position,
              rotate, current_transform_component->scale);
          auto droatate = rotate - current_transform_component->rotation;
          current_transform_component->rotation += droatate;
        }
      }
    }
  }

  void EditorLayer::serializeEditor(const char* path)
  {
    EditorSerializer es(this);
    if(!es.serialize(path))
      REAL_ASSERT(false);
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

  void EditorLayer::onEvent(Event& e)
  {
    EventDispatcher dp(e);
    dp.dispatch<MouseScrolledEvent>(BIND_EVENT_FUNCTION(EditorLayer::onMouseScrolledEvent));
    dp.dispatch<KeyPressedEvent>(BIND_EVENT_FUNCTION(EditorLayer::onKeyPressedEvent));
    dp.dispatch<KeyReleasedEvent>(BIND_EVENT_FUNCTION(EditorLayer::onKeyReleasedEvent));
  }

  bool EditorLayer::onKeyPressedEvent(KeyPressedEvent& e)
  {
    return false;
  }

  bool EditorLayer::onKeyReleasedEvent(KeyReleasedEvent& e)
  {
    return false;
  }

  bool EditorLayer::onMouseScrolledEvent(MouseScrolledEvent& e)
  {
    m_camera.onMouseScrolledEvent(e);
    return false;
  }

  void EditorLayer::handelCMD(int cmdkey)
  {
    if(Input::isKeyPressed(Keyboard::Q))
      m_op = GizmoOp::Bounds;
    else if(Input::isKeyPressed(Keyboard::W))
      m_op = GizmoOp::Translate;
    else if(Input::isKeyPressed(Keyboard::R))
      m_op = GizmoOp::Rotate;
    else if(Input::isKeyPressed(Keyboard::S))
      m_op = GizmoOp::Scale;
    else if(Input::isKeyPressed(Keyboard::U))
      m_op = GizmoOp::Universal;
    else if(Input::isKeyPressed(Keyboard::D))
    {
      auto e = m_scene_hierarchy_panel.getCurrentSelected();
      if(e)
        m_scene->copyEntity(e);
    }
  }
}
