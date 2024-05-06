#ifndef KR_EDITOR_LAYER_H
#define KR_EDITOR_LAYER_H


#include <Karen/Karen.h>
#include "Panels/SceneHierarchy.h"
#include "Panels/Inspector.h"
#include "HelperWindows.h"


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

    void serializeEditor(const char* path);
    
    void deSerializeEditor(const char* path);
  private:
    //TODO: Serizeation data as struct or some thing 
    bool m_default_editor;
    std::string m_default_font = "../res/fonts/Roboto/Roboto-Regular.ttf";
    uint8_t m_default_font_size = 18;
    Timestep m_time_step;
    ARef<Scene> m_scene;
    std::string m_imgui_ini_path;
    Vec2 m_viewport_size = {0.0f, 0.0f};
    Vec2 m_quad_pos = {0.0f, 0.0f};
    ARef<Karen::FrameBuffer> m_frame_buff;
    std::unordered_map<std::string, Scoped<HelperWindow>> m_helper_windows;
    std::unordered_map<std::string, Vec4> m_colors;
    SceneHierarchy m_scene_hierarchy_panel;
    Inspector m_inspector_panel;
  private:
    friend class EditorSerializer;
    friend class MenuBar;
  };
}

#endif //KR_EDITOR_LAYER_H
