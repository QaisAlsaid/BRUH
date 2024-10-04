#include <filesystem>
#include <pch.h>
#include "ContentBrowser.h"
#include "Real-Engine/Core/AssetManager.h"
#include "Real-Engine/Render/API/Texture.h"
#include "imgui.h"

namespace Real
{   
  ContentBrowser::ContentBrowser(const std::string& res_dir)
    : m_res_dir(res_dir) 
  {
    auto tex = Texture2D::create("../res/textuers/folder_icon.png");
    
    m_names["Scene"] = UUID();
    m_names["Script"] = UUID();
    m_names["Cpp"] = UUID();
    m_names["H"] = UUID();
    m_names["Yaml"] = UUID();
    m_names["Xml"] = UUID();
    m_names["Glsl"] = UUID();
    m_names["Dir"] = UUID();
    m_names["Other"] = UUID();

    m_default_icons[m_names.at("Scene")] = tex;
    m_default_icons[m_names.at("Script")] = tex;
    m_default_icons[m_names.at("Cpp")] = tex;
    m_default_icons[m_names.at("H")] = tex;
    m_default_icons[m_names.at("Yaml")] = tex;
    m_default_icons[m_names.at("Xml")] = tex;
    m_default_icons[m_names.at("Glsl")] = tex;
    m_default_icons[m_names.at("Dir")] = tex;
    m_default_icons[m_names.at("Other")] = tex;
    for(auto& entry : std::filesystem::recursive_directory_iterator(m_res_dir))
    {
      if(!entry.is_directory() && !entry.is_fifo() && !entry.is_symlink() && !entry.is_socket())
      {
        loadOrGet(entry.path(), getFileType(entry));
      }
    }
    m_current_dir = m_res_dir;
    REAL_CORE_WARN("EXIT ContentBrowser CTOR");
  }

  void ContentBrowser::onImGuiUpdate()
  {
    ImGui::Begin("Content Browser");
    static float thm_width = 100.0f; 
    static float padding = 16.0f;
    float cell_size = thm_width + padding;
    float panel_width = ImGui::GetContentRegionAvail().x;
    int cols = panel_width / cell_size;
    if(cols < 1) cols = 1;
   

    if(m_current_dir != m_res_dir)
    {
      if(ImGui::Button("<-"))
      {
        m_current_dir = m_current_dir.parent_path();
      }
    }
    
    ImGui::Columns(cols, 0, false);
    for(auto& entry : std::filesystem::directory_iterator(m_current_dir))
    {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
      const auto& path = entry.path();
      const auto& file_name = path.filename();
      const auto& relative_path = std::filesystem::relative(path, m_res_dir);
      auto ft = getFileType(entry);

      auto thm_id = loadOrGet(path, ft);

      UUID asset_id = UUID::invalid;
      if(ft != FileType::Dir && isAsset(ft)) //for now
        asset_id = AssetManager::getUUID(path);

      if(ft != FileType::Image)
      {
        ImGui::ImageButton((ImTextureID)(uintptr_t)m_default_icons.at(thm_id)->getRendererID(),
            {thm_width, thm_width}, {0, 1}, {1, 0});
        if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && entry.is_directory())
        {
          m_current_dir /= file_name;
        }
      }
      else 
      {
        ImGui::ImageButton((ImTextureID)(uintptr_t)AssetManager::get<AssetManager::Texture2DAsset>(asset_id)->texture->getRendererID(), {thm_width, thm_width}, {0, 1}, {1, 0});
      }
      if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && isAsset(ft))
      {
        m_asset_manager_modal.setContext(asset_id);
        m_asset_manager_modal.show();
      }
      if(isAsset(ft))
      {
        if(ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
          REAL_CORE_WARN("Drag and drop started handle: {0}", asset_id);
          switch(ft)
          {
            case FileType::Image:
              dragAndDropTexture(asset_id , {50, 50});
              break;
            case FileType::Scene:
              dragAndDropScene(asset_id, {50, 50});
              break;
            case FileType::Script:
              dragAndDropScript(asset_id, {50, 50});
              break;
            default: break;
          }
        }
      }
      ImGui::PopStyleColor();
      std::string file_name_str = file_name.string();
      ImGui::TextWrapped("%s", file_name_str.c_str());
      ImGui::NextColumn();
    }

    ImGui::Columns(1);
    
    ImGui::End();

    m_asset_manager_modal.onImGuiUpdate();

  }

  UUID ContentBrowser::loadOrGet(const std::filesystem::path& entry, FileType ft)
  {
    AssetManager::Asset::Meta meta;
    meta.path = entry.string();
    switch(ft)
    {
      case FileType::Image:
      {
        meta.type = AssetManager::Asset::Type::Texture2D;
        return AssetManager::loadOrGet(meta);
      }
      case FileType::Script:
      {
        meta.type = AssetManager::Asset::Type::Script;
        if(AssetManager::loadOrGet(meta))
          return getDefaultIcon(ft);
        else return UUID::invalid;
      }
      case FileType::Scene:
      {
        meta.type = AssetManager::Asset::Type::Scene;
        if(AssetManager::loadOrGet(meta))
          return getDefaultIcon(ft);
        else return UUID::invalid;
      }
      default: return getDefaultIcon(ft);
    }
  }

  UUID ContentBrowser::getDefaultIcon(FileType ft)
  {
    switch(ft)
    {
      case FileType::Image:  return UUID::invalid;
      case FileType::Scene:  return m_names.at("Scene");
      case FileType::Script: return m_names.at("Script");
      case FileType::Dir:    return m_names.at("Dir");
      case FileType::H:      return m_names.at("H");
      case FileType::Cpp:    return m_names.at("Cpp");
      case FileType::Xml:    return m_names.at("Xml");
      case FileType::Glsl:   return m_names.at("Glsl");
      case FileType::Yaml:   return m_names.at("Yaml");
      default:               return m_names.at("Other");
    }
  }

  bool ContentBrowser::isAsset(FileType ft)
  {
    if(ft == FileType::Image || ft == FileType::Scene || ft == FileType::Script)
      return true;
    return false;
  }

  ContentBrowser::FileType ContentBrowser::getFileType(const std::filesystem::directory_entry& entry)
  {
    if(entry.is_directory()) return FileType::Dir;

    const auto& file_ex = entry.path().extension();
    if(file_ex == ".Real") return FileType::Real;
    if(file_ex == ".png") return FileType::Png;
    if(file_ex == ".jpeg") return FileType::Jpeg;
    if(file_ex == ".jpg") return FileType::Jpg;
    if(file_ex == ".yaml") return FileType::Yaml;
    if(file_ex == ".xml") return FileType::Xml;
    if(file_ex == ".cpp" || file_ex == ".cxx" || file_ex == ".cc") return FileType::Cpp;
    if(file_ex == ".h" || file_ex == ".hpp" || file_ex == ".hxx") return FileType::H;
    if(file_ex == ".lua") return FileType::Lua;
    if(file_ex == ".glsl") return FileType::Glsl;
    else return FileType::Other;
  }

  void ContentBrowser::dragAndDropTexture(UUID id, const Vec2& thm_size)
  {
    auto rid = AssetManager::get<AssetManager::Texture2DAsset>(id)->texture->getRendererID();
    ImGui::SetDragDropPayload("TEXTURE2D_ASSET_HANDEL", &id, sizeof(UUID));
    ImGui::Image((void*)(uintptr_t)rid, { thm_size.x, thm_size.y }, {0, 1}, {1, 0});
    ImGui::EndDragDropSource();
  }

  void ContentBrowser::dragAndDropScene(UUID id, const Vec2& thm_size)
  {
    //auto rid = AssetManager::get<AssetManager::SceneAsset>(id);
    //auto rid = getDefaultIcon(FileType::Scene);
    ImGui::Image((void*)(uintptr_t)m_default_icons.at(getDefaultIcon(FileType::Scene))->getRendererID(),
        { thm_size.x, thm_size.y }, {0, 1}, {1, 0});
    ImGui::SetDragDropPayload("SCENE_ASSET_HANDEL", &id, sizeof(UUID));
    //ImGui::Image((void*)(uintptr_t)rid, { thm_size.x, thm_size.y }, {0, 1}, {1, 0});
    ImGui::EndDragDropSource();
  }
  
  void ContentBrowser::dragAndDropScript(UUID id, const Vec2& thm_size)
  {
    ImGui::SetDragDropPayload("SCRIPT_ASSET_HANDEL", &id, sizeof(UUID));
    ImGui::Image((void*)(uintptr_t)m_default_icons.at(getDefaultIcon(FileType::Script))->getRendererID(),
        { thm_size.x, thm_size.y }, {0, 1}, {1, 0});
    ImGui::EndDragDropSource();
  }
 
}
