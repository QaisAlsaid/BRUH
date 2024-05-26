#ifndef KR_ASSET_MANAGER_H
#define KR_ASSET_MANAGER_H

#include "Karen/Core/Core.h"
#include "Karen/Render/API/Texture.h"
#include "Karen/Scene/Scene.h"
#include <string_view>
#include <unordered_map>


namespace Karen
{
  
  class KAREN_API AssetManager
  {
  public:
    struct KAREN_API Asset 
    {
      std::string path;
    };

    struct KAREN_API Texture2DAsset : public Asset
    {
      ARef<Texture2D> texture;
    };

    struct KAREN_API SceneAsset : public Asset 
    {
      ARef<Scene> scene;
    };

  public:
    AssetManager() = default;
    AssetManager(const char* config_path);

    bool loadConfig(const char* path);
    
    void reload();
    void reloadTexture(const std::string& name);
    void addTexture(const std::string& name, const ARef<Texture2D>& tux, const std::string& path);
    
    inline const ARef<Texture2D>& getTexture2D(const std::string& name) const
    {
      KAREN_CORE_ASSERT_MSG(m_textures.find(name) != m_textures.end(), "Texture: " + std::string(name) + " Not Found");
      return (m_textures.at(name).texture);
    }

    inline void deleteTexture(const std::string& name)
    {
      auto iter = m_textures.find(name);
      if(iter != m_textures.end())
        m_textures.erase(iter);
    }

    inline void clearTextures() { m_textures.clear(); }
     
    
    void reloadScene(const std::string& name);
    void addScene(const std::string& name, const ARef<Scene>& scene, const std::string& path);
    
    inline const ARef<Scene>& getScene(const std::string& name) const
    {
      KAREN_CORE_ASSERT_MSG(m_scenes.find(name) != m_scenes.end(), "Scene: " + std::string(name) + " Not Found");
      return (m_scenes.at(name).scene);
    }

    inline void deletescene(const std::string& name)
    {
      auto iter = m_scenes.find(name);
      if(iter != m_scenes.end())
        m_scenes.erase(iter);
    }

    inline void clearScenes() { m_scenes.clear(); }

    inline void clear() 
    {
      m_scenes.clear();
      m_textures.clear();
    }

    inline const std::unordered_map<std::string, Texture2DAsset>& Textures() const { return m_textures; }

    inline const std::unordered_map<std::string, SceneAsset>& Scenes() const { return m_scenes;}
  private:
    std::unordered_map<std::string, SceneAsset>     m_scenes;
    std::unordered_map<std::string, Texture2DAsset> m_textures;
  };
}



#endif //KR_ASSET_MANAGER_H
