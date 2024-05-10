#ifndef KR_ASSET_MANAGER_H
#define KR_ASSET_MANAGER_H

#include "Karen/Core/Core.h"
#include "Karen/Render/API/Texture.h"
#include "Karen/Scene/Scene.h"
#include <string_view>
#include <unordered_map>


namespace Karen
{
  class AssetManager
  {
  public:
    AssetManager() = default;
    AssetManager(const char* config_path);

    bool loadConfig(const char* path);
    
    void addTexture(const std::string& name, const ARef<Texture2D>& tux);
    
    inline const ARef<Texture2D>& getTexture2D(const std::string& name) const
    {
      KAREN_CORE_ASSERT_MSG(m_textures.find(name) != m_textures.end(), "Texture: " + std::string(name) + " Not Found");
      return (m_textures.at(name));
    }

    void addScene(const std::string& name, const ARef<Scene>& scene);
    inline const ARef<Scene>& getScene(const std::string& name) const
    {
      KAREN_CORE_ASSERT_MSG(m_scenes.find(name) != m_scenes.end(), "Scene: " + std::string(name) + " Not Found");
      return (m_scenes.at(name));
    }
  private:
    std::unordered_map<std::string, ARef<Scene>>   m_scenes;
    std::unordered_map<std::string, ARef<Texture2D>> m_textures;
  };
}



#endif //KR_ASSET_MANAGER_H
