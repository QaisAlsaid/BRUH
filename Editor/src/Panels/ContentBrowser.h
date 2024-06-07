#ifndef KR_CONTENT_BROWSER_H
#define KR_CONTENT_BROWSER_H

#include "Karen/Karen.h"
#include "AssetManagerGui.h"
#include <filesystem>
#include <vector>


namespace Karen
{
  class ContentBrowser
  {
  public:
    ContentBrowser(const std::string& res_dir);//TODO: Takes project
    
    void onImGuiUpdate();
    std::string getResDir() { return m_res_dir; };
  private:
    enum class FileType 
    {
      Karen, Png, Jpeg, Jpg, Yaml, Xml, Cpp, H, Lua, Glsl, Other
    };
    ARef<Texture2D> loadOrGet(const std::filesystem::path& tux_path); //TODO: template when you have m_thumbnails for scenes or models
    ARef<Texture2D> getDefaultIcon(const FileType file_type);
    FileType getFileType(const std::filesystem::path& file_path);
    bool isImage(const FileType file_type);
  private:
    AssetManagerModal            m_asset_manager_modal;
    std::filesystem::path        m_current_dir;
    const std::string            m_res_dir;
    std::unordered_map<std::string, ARef<Texture2D>>    m_thumbnails;
  };
}

#endif //KR_CONTENT_BROWSER_H
