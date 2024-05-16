#include <pch.h>
#include "AssetManagerGui.h"
#include "Karen/Karen.h"

#include <imgui.h>


namespace Karen
{
  static void drawTextuerStats(const char* name, float aspect_ratio, float height, const ARef<Texture2D>& tux, AssetManager* am);
  static void dragAndDropTex(const char* name, const uint32_t name_len, uint32_t id, const Vec2& thm_size);
  //////////////////////////////Panel///////////////////////////////////////////
  AssetManagerPanel::AssetManagerPanel(AssetManager* ctx)
    : m_context(ctx)
  {
  }

  void AssetManagerPanel::onImGuiUpdate()
  {
    if(!m_is_active) return;
    
    ImGui::Begin("Asset Manager");
    
    static float thm_width = 100.0f; 
    static float padding = 16.0f;
    float cell_size = thm_width + padding;
    float panel_width = ImGui::GetContentRegionAvail().x;
    int cols = panel_width / cell_size;
    if(cols < 1) cols = 1;

    if(m_context)
    {
      if(ImGui::Button("Reload"))
        m_context->reload();
      ImGui::SameLine();
      if(ImGui::Button("Clear"))
      {
        ImGui::OpenPopup("good bye assets");
      }
      if(ImGui::BeginPopup("good bye assets"))
      {
        if(ImGui::Button("Ok"))
        {
          m_context->clear();
          ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel"))
          ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
        //TODO: dont show again
      }
      ImGui::Columns(cols, 0, false);
      for(const auto& iter : m_context->Textures())
      {
        const char* asset_name = iter.first.c_str();
        float aspect_ratio = (float)iter.second.texture->getWidth()/(float)iter.second.texture->getHeight();

        const auto flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        bool opend = ImGui::TreeNodeEx(asset_name, flags);
        if(ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
          dragAndDropTex(asset_name, strlen(asset_name), iter.second.texture->getRendererID(), {50 * aspect_ratio, 50});
        }
        if(opend)
        {
          drawTextuerStats(asset_name, aspect_ratio, thm_width, iter.second.texture, m_context);
          ImGui::TreePop();
        }
      ImGui::NextColumn();
      }
    }
    else
      ImGui::TextColored(ImVec4(0.75, 0.175, 0.175, 1.0f), "No Context Specified for this Panel !?");
    ImGui::Columns(1);
    ImGui::End();
  }

  static void drawTextuerStats(const char* name, float aspect_ratio, float height, const ARef<Texture2D>& tux, AssetManager* am)
  {
    if(ImGui::Button("Reload"))
      am->reloadTexture(name);
    ImGui::Text("Use Count: %li", tux.use_count());
    ImVec2 cra = ImGui::GetContentRegionAvail();
    ImGui::Image((void*)(uintptr_t)tux->getRendererID(), {height, height}, {0, 1}, {1, 0});
  }


  static void dragAndDropTex(const char* name, uint32_t name_len, uint32_t id, const Vec2& thm_size)
  {
    ImGui::SetDragDropPayload("TEXTURE2D_ASSET_HANDEL", name, sizeof(char) * strlen(name));
    ImGui::Image((void*)(uintptr_t)id, { thm_size.x, thm_size.y }, {0, 1}, {1, 0});
    ImGui::EndDragDropSource();
  }


  /////////////////////////////////////Modal////////////////////////////////////////////////////

  AssetManagerModal::AssetManagerModal(AssetManager* ctx)
    : m_manager_context(ctx)
  {

  }

  void AssetManagerModal::onImGuiUpdate()
  {
    static std::string name = "Asset";
    if(m_is_active)
      ImGui::OpenPopup("Add Asset");
    if(ImGui::BeginPopupModal("Add Asset", NULL, ImGuiWindowFlags_MenuBar))
    {
      char buff[256];
      memset(buff, 0, sizeof(buff));
      strcpy(buff, name.c_str());
      if(ImGui::InputText("Asset Name", buff, sizeof(buff)))
        name = std::string(buff);
      if(ImGui::Button("Load"))
      {
        if(!name.empty())
        {
          switch(m_to_load_context.type)
          {
            case TypeEnum::Texture2D:
              if(m_manager_context->Textures().find(name) != m_manager_context->Textures().end())
              {
                ImGui::OpenPopup("Enter Unique Name!");
              }
              else 
              {
                m_manager_context->addTexture(name, Texture2D::create(m_to_load_context.path), m_to_load_context.path);
                KAREN_CORE_WARN("Loaded");
                m_is_active = false;
                ImGui::CloseCurrentPopup();
              }
              break;
            default: break;
          }
        }
        else 
        {
          ImGui::OpenPopup("Enter Unique Name!");
        }
      }
      if(ImGui::Button("Cancel"))
      {
        m_is_active = false;
        ImGui::CloseCurrentPopup();
      }
      if(ImGui::BeginPopupModal("Enter Unique Name!"))
      {
        ImGui::TextColored(ImVec4(0.75f, 00.175f, 0.175f, 1.0f), "Enter A Unique Asset Name!");
        if(ImGui::Button("Ok"))
          ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
      }
      ImGui::EndPopup();
    }
  }
}
