#include "Karen/Render/API/Texture.h"
#include "pch.h"
#include "ContentBrowser.h"


#include <filesystem>
#include <imgui.h>

namespace Karen
{
  //TODO: All paths must be relative to res dir and all names in the map shold be the full path from the res dir 
  //for delete the deleted stuff(from desk) from the map
  //TODO: Everything should have a timer
  ContentBrowser::ContentBrowser(const std::string& rp)
    :m_asset_manager_modal(App::get()->assetManager()), m_current_dir(rp), m_res_dir(rp)
  {
    uint32_t counter = 0;
    for(auto& entry : std::filesystem::recursive_directory_iterator(m_res_dir))
    {
      if(!entry.is_directory() && !entry.is_fifo() && !entry.is_symlink() && !entry.is_socket())
      {
        if(isImage(getFileType(entry.path())))
          counter++;
      }
    }
    
    m_thumbnails.reserve(counter);
    
    for(auto& entry : std::filesystem::recursive_directory_iterator(m_res_dir))
    {
      if(!entry.is_directory() && !entry.is_fifo() && !entry.is_symlink() && !entry.is_socket())
      {
        if(isImage(getFileType(entry.path())))
          loadOrGet(entry.path());
      }
    }
    //TODO: when the res folder is only for the project files load the folder image and default icons alone
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
      if(entry.is_directory())
      {
        ImGui::ImageButton((ImTextureID)(uintptr_t)m_thumbnails.at("folder_icon.png")->getRendererID(), {thm_width, thm_width}, {0, 1}, {1, 0});
        if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
          m_current_dir /= file_name;
        }
      }
      else 
      {
        const auto type = getFileType(file_name);
        const auto icon = isImage(type) ? loadOrGet(path) : getDefaultIcon(type);
        ImGui::ImageButton((ImTextureID)(uintptr_t)icon->getRendererID(), {thm_width, thm_width}, {0, 1}, {1, 0});
        if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && isImage(type)/*&& type.is_loadable*/)
        {
          m_asset_manager_modal.setToLoadContext({ path, AssetManagerModal::TypeEnum::Texture2D });
          m_asset_manager_modal.show();
          //TODO: show AssetManager Loading dialog
        }
      }
      ImGui::PopStyleColor();
      ImGui::TextWrapped("%s", file_name.string().c_str());
      ImGui::NextColumn();
    }

    ImGui::Columns(1);
    
    ImGui::End();
    
    ImGui::Begin("__DEBUG__");
    
    ImGui::SliderFloat("Icon Size", &thm_width, 16, 512);
    ImGui::SliderFloat("Padding", &padding, 0, 32);
    uint32_t counter = 0;
    for(auto& x : m_thumbnails) ++counter;
    ImGui::Text("%u", counter);

    ImGui::End();
    m_asset_manager_modal.onImGuiUpdate();
    //TODO: 
    /*
    for(const auto& iter : m_thumbnails)
    {
      for(auto& entry : std::filesystem::directory_iterator(m_current_dir))
      {
        const auto& iter = m_thumbnails.find(entry.path().filename());
        if(iter != m_thumbnails.end())
        {
          m_thumbnails.erase(iter);
        }
      }
    }
    */
  }


  ContentBrowser::FileType ContentBrowser::getFileType(const std::filesystem::path& file_path)
  {
    const auto& file_ex = file_path.extension();
    if(file_ex == ".Karen") return FileType::Karen;
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

  bool ContentBrowser::isImage(const FileType ft)
  {
    switch(ft)
    {
      case FileType::Png  : return true;
      case FileType::Jpeg : return true;
      case FileType::Jpg  : return true;
      default             : return false;
    }
  }

  ARef<Texture2D> ContentBrowser::loadOrGet(const std::filesystem::path& p)
  {
    if(m_thumbnails.find(p.filename()) != m_thumbnails.end())
      return m_thumbnails.at(p.filename());
    else
    {
      const auto& ct = Texture2D::create(p);
      m_thumbnails[p.filename()] = ct;
      return ct;
    }
  }

  ARef<Texture2D> ContentBrowser::getDefaultIcon(const FileType ft)
  {
    switch(ft)
    {
      default : return m_thumbnails.at("folder_icon.png");
    }
  }
}
