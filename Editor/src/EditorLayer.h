#ifndef KR_EDITOR_LAYER_H
#define KR_EDITOR_LAYER_H


#include <Karen/Karen.h>
#include "Panels/SceneHierarchy.h"
#include "Panels/Inspector.h"
#include "HelperWindows.h"
#include "Panels/ContentBrowser.h"
#include "Panels/AssetManagerGui.h"
#include "EditorCamera.h"


namespace Karen
{
  class EditorLayer : public Karen::Layer
  {
  public:
    enum class SceneState {Stop, Play};
    enum class GizmoOp 
    {
      TranslateX       = (1u << 0),
      TranslateY       = (1u << 1),
      TranslateZ       = (1u << 2),
      RotateX          = (1u << 3),
      RotateY          = (1u << 4),
      RotateZ          = (1u << 5),
      RotateCamera     = (1u << 6),
      ScaleX           = (1u << 7),
      ScaleY           = (1u << 8),
      ScaleZ           = (1u << 9),
      Bounds           = (1u << 10),
      ScaleXU          = (1u << 11),
      ScaleYU          = (1u << 12),
      ScaleZU          = (1u << 13),

      Translate = TranslateX | TranslateY | TranslateZ,
      Rotate = RotateX | RotateY | RotateZ | RotateCamera,
      Scale = ScaleX | ScaleY | ScaleZ,
      ScaleU = ScaleXU | ScaleYU | ScaleZU,
      Universal = Translate | Rotate | ScaleU
    };
  public: 
    EditorLayer();
  
    void onAttach() override;

    void onDetach() override;

    void onUpdate(Karen::Timestep ts) override;

    void onEvent(Karen::Event& e) override;

    void onGuiUpdate() override;
  private:
    void onScenePlay();
    void onSceneStop();
    
    void serializeEditor(const char* path);
    void deSerializeEditor(const char* path);
    
    void setColorScheme();
    void initImGui();

    void updateMenuBar();
    void updatePanels();
    void updateGizmos(Vec2 panel_pos, Vec2 panel_size);

    bool onMouseScrolledEvent(MouseScrolledEvent& e);
    bool onKeyPressedEvent(KeyPressedEvent& e);
    bool onKeyReleasedEvent(KeyReleasedEvent& e);
    void handelCMD(int key);
  private:
    Timestep m_time_step;
    SceneState m_scene_state = SceneState::Stop;
    bool m_show_imgui_demo = true;
    bool m_default_editor = false;
    std::string m_default_font;
    uint8_t m_default_font_size = 18;
    EditorCamera m_camera;

    std::string m_imgui_ini_path;
    Vec2 m_viewport_size = {0.0f, 0.0f};
    Vec2 m_min_vp_bounds, m_max_vp_bounds;
    ARef<Scene> m_scene;
    ARef<Scene> m_editor_scene;
    GizmoOp m_op = GizmoOp::Bounds;
    
    ARef<Karen::FrameBuffer> m_frame_buff;
    
    AssetManagerPanel m_asset_manager_panel;
    ContentBrowser m_content_browser;
    SceneHierarchy m_scene_hierarchy_panel;
    Inspector m_inspector_panel;

    std::unordered_map<std::string, Scoped<HelperWindow>> m_helper_windows;
    std::unordered_map<std::string, Vec4> m_colors;
  private:
    friend class EditorSerializer;
    friend class MenuBar;
  };
}

#endif //KR_EDITOR_LAYER_H
