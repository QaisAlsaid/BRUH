#ifndef ASSET_MANAGER_H_test
#define ASSET_MANAGER_H_test


#include "Karen/Core/Assertion.h"
#include "Karen/Core/Core.h"
#include "Karen/Core/UUID.h"
#include "Karen/Render/API/Texture.h"
#include "Karen/Scene/Scene.h"
namespace Karen::Test
{
  class AssetManager
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
    AssetManager(const char* config_path);
    
    inline void attachName(UUID asset_id, const std::string& name) { m_names[name] = asset_id; };
    
    inline const ARef<Texture2D>& getTexture2D(UUID handle) const
    {
      KAREN_CORE_ASSERT_MSG(m_assets.find(handle) != m_assets.end(), "No Matching Asset With Handle: {0}", (uint64_t)handle);
      return std::static_pointer_cast<Texture2DAsset>(m_assets.at(handle))->texture;
    }

    inline const ARef<Scene>& getScene(UUID handle) const
    {
      KAREN_CORE_ASSERT_MSG(m_assets.find(handle) != m_assets.end(), "No Matching Asset With Handle: {0}", (uint64_t)handle);
      return std::static_pointer_cast<SceneAsset>(m_assets.at(handle))->scene;
    }

    inline const ARef<Texture2D>& getTexture2D(const std::string& name) const
    {
      KAREN_CORE_ASSERT_MSG(m_names.find(name) != m_names.end(), "No Matching Asset With Name: {0}", name);
      auto handle = m_names.at(name);
      KAREN_CORE_ASSERT_MSG(m_assets.find(handle) != m_assets.end(), "No Matching Asset With Handle: {0}", (uint64_t)handle);
      return std::static_pointer_cast<Texture2DAsset>(m_assets.at(handle))->texture;
    }

    inline const ARef<Scene>& getScene(const std::string& name) const
    {
      KAREN_CORE_ASSERT_MSG(m_names.find(name) != m_names.end(), "No Matching Asset With Name: {0}", name);
      auto handle = m_names.at(name);
      KAREN_CORE_ASSERT_MSG(m_assets.find(handle) != m_assets.end(), "No Matching Asset With Handle: {0}", (uint64_t)handle);
      return std::static_pointer_cast<SceneAsset>(m_assets.at(handle))->scene;
    }

  private:
    std::unordered_map<UUID, ARef<Asset*>> m_assets;
    std::unordered_map<std::string, UUID>  m_names;
  };
}


#endif //ASSET_MANAGER_H
