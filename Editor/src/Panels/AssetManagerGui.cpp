#include <pch.h>
#include "AssetManagerGui.h"
#include "Karen/Karen.h"

#include <imgui.h>


namespace Karen
{
  static void drawTextuerStats(UUID id)
  {
    if(ImGui::Button("Reload"))
      AssetManager::reload(id);
    auto tex = AssetManager::get<AssetManager::Texture2DAsset>(id)->texture;
    ImGui::Text("Use Count: %li", tex.use_count());
    ImVec2 cra = ImGui::GetContentRegionAvail();
    ImGui::Image((void*)(uintptr_t)tex->getRendererID(), ImVec2(tex->getWidth(), tex->getHeight()), {0, 1}, {1, 0});
  }

  static void drawSceneStats(UUID id)
  {
    if(ImGui::Button("Reload"))
      AssetManager::reload(id);
    
    auto scene = AssetManager::get<AssetManager::SceneAsset>(id)->scene;
    //TODO: render the scene to image
    /*
    scene->onEditorUpdate(0.2);
    const ImVec2 panel_size = ImGui::GetContentRegionAvail();
    Vec2 k_panel_size = {panel_size.x, panel_size.y};
    
    frame_buff->reSize(k_panel_size.x, k_panel_size.y);
    
    m_scene->onViewportResize(panel_size.x, panel_size.y);
    camera.onResize(panel_size.x, panel_size.y);
    
    m_camera.aspect_ratio = m_viewport_size.x/m_viewport_size.y;
    ImGui::Image((void*)(uintptr_t)m_frame_buff->getColorAttachmentId("render_buffer"), panel_size, ImVec2(0, 1), ImVec2(1, 0));
*/
    ImGui::Text("Use Count: %li", scene.use_count());
  }

  static void drawScriptStats(UUID id)
  {
    if(ImGui::Button("Reload"))
      AssetManager::reload(id);
    
    auto script = AssetManager::get<AssetManager::ScriptAsset>(id)->script;
    ImGui::Text("Use Count: %li", script.use_count());
  }

  void AssetManagerModal::show(UUID id)
  {
    static std::string name = "Asset";
    ImGui::OpenPopup("AssetStats");
    if(ImGui::BeginPopupModal("AssetStats", NULL, ImGuiWindowFlags_MenuBar))
    {
      auto type = AssetManager::get(id)->meta.type;
      switch(type)
      {
        case AssetManager::Asset::Type::None: return;
        case AssetManager::Asset::Type::Texture2D: drawTextuerStats(id);
        case AssetManager::Asset::Type::Scene: drawSceneStats(id);
        case AssetManager::Asset::Type::Script: drawScriptStats(id);
      }
      if(ImGui::Button("Cancel"))
        ImGui::CloseCurrentPopup();
      ImGui::EndPopup();
    }
  }
}
