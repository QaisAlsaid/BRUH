#ifndef KR_EDITOR_LAYER_H
#define KR_EDITOR_LAYER_H


#include <Karen/Karen.h>
#include "Panels/SceneHierarchy.h"
#include "Panels/Inspector.h"
#include "HelperWindows.h"
#include "Panels/ContentBrowser.h"
#include "Panels/AssetManagerGui.h"

namespace Karen
{
  class EditorLayer : public Karen::Layer
  {
  public:
    enum class SceneState {Stop, Play};
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
  private:
    Timestep m_time_step;
    SceneState m_scene_state = SceneState::Stop;
    bool m_show_imgui_demo = true;
    bool m_default_editor = false;
    std::string m_default_font;
    uint8_t m_default_font_size = 18;
    
    std::string m_imgui_ini_path;
    Vec2 m_viewport_size = {0.0f, 0.0f};
    
    ARef<Scene> m_scene;
    
    ARef<Karen::FrameBuffer> m_frame_buff;
    
    AssetManagerPanel m_asset_manager_panel;
    ContentBrowser m_content_browser;
    SceneHierarchy m_scene_hierarchy_panel;
    Inspector m_inspector_panel;

    std::unordered_map<std::string, Scoped<HelperWindow>> m_helper_windows;
    std::unordered_map<std::string, Vec4> m_colors;
  private:
    void serializeEditor(const char* path);
    void deSerializeEditor(const char* path);
    
    void setColorScheme();
    void initImGui();

    void updateMenuBar();
    void updatePanels();
  private:
    friend class EditorSerializer;
    friend class MenuBar;
  };
}

#endif //KR_EDITOR_LAYER_H
