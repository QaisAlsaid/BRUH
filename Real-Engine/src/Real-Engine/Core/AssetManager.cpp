#include "AssetManager.h"
#include "Real-Engine/Scene/Components.h"
#include "Real-Engine/Scene/Scene.h"
#include "Real-Engine/Scene/SceneSerializer.h"
#include "Real-Engine/Scripting/Lua.h"
#include "Real-Engine/Scripting/Script.h"

#include <pugixml.hpp>
#include <string>
#include <system_error>


namespace Real
{
  const ARef<AssetManager::Asset> AssetManager::Asset::invalid = createARef<AssetManager::Asset>(false);
  AssetManager* AssetManager::s_instance = new AssetManager;

  bool AssetManager::loadConfig(const std::string& path)
  {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.c_str(),
    pugi::parse_default | pugi::parse_declaration);
    if (!result)
    {
      REAL_CORE_ERROR("Parse Error at: {0}, offset: {1}",result.description(), result.offset);
      return false;
    }
    
    auto assets = doc.child("Assets");
    for(auto asset : assets)
    {
      std::string uuid_str = asset.child_value("UUID");
      if(uuid_str.empty())
      {
        REAL_CORE_WARN("(AssetManager): Trying to Load Asset with no UUID");
        continue;
      }
      auto meta_n = asset.child("Meta");
      std::string path = meta_n.child_value("Path");
      if(path.empty())
      {
        REAL_CORE_WARN("(AssetManager): Trying to Load Asset: {0} with no Meta::Path", uuid_str);
        continue;
      }
      std::string type_str = meta_n.child_value("Type");
      if(type_str.empty())
      {
        REAL_CORE_WARN("(AssetManager): Trying to Load Asset: {0} with no Meta::Type", uuid_str);
        continue;
      }
      
      UUID uuid = std::stoll(uuid_str);
      if(uuid == UUID::invalid) uuid = UUID();
      Asset::Type type = Asset::Type::None;
      if(type_str == "Texture2D") type = Asset::Type::Texture2D; 
      else if(type_str == "Scene") type = Asset::Type::Scene;
      else if(type_str == "Script") type = Asset::Type::Script;
      else { REAL_CORE_WARN("(AssetManager): invalid Asset::Type from Asset: {0}", uuid_str); continue; }

      Asset::Meta meta;
      meta.path = path;
      meta.type = type;
      REAL_CORE_WARN("START LOADING ASSET: {0}", meta.path);
      load(meta, uuid);
      REAL_CORE_WARN("DONE");
    }
 
    return true;
  }

  bool AssetManager::AssetLoader::loadTexture2D(const ARef<Texture2DAsset>& asset)
  {
    asset->texture = Texture2D::create(asset->meta.path);
    if(asset->onReload)
      asset->onReload();
    return true;
  }

  bool AssetManager::AssetLoader::loadScene(const ARef<SceneAsset>& asset)
  {
    asset->scene = createARef<Scene>();
    SceneSerializer ss(asset->scene);
    if(!ss.deSerializeText(asset->meta.path.c_str()))
      return false;
    if(asset->onReload)
      asset->onReload();
    return true;
  }

  bool AssetManager::AssetLoader::loadScript(const ARef<ScriptAsset>& asset)
  {
    auto& lua = Lua::get();
    lua.safe_script_file(asset->meta.path);
    auto res = lua["GetObject"];
    if(res.valid())
    {
      sol::function fun = res;
      asset->script = fun();
    }
    else 
    {
      sol::error e = res;
      REAL_CORE_ERROR("{0}", e.what());
      return false;
    }
    if(asset->onReload)
      asset->onReload();
    return true;
  }

  UUID AssetManager::add(const ARef<Asset>& asset, UUID uuid, const std::string& path)
  {
    UUID id = uuid;
    if(s_instance->m_assets.find(uuid) != s_instance->m_assets.end())
    {
      id = UUID();
      add(asset, id, path);
    }
    if(s_instance->m_paths.find(path) != s_instance->m_paths.end())
    {
      REAL_CORE_ERROR("(AssetManager): Asset With Path: {0} already exist", path);
      return UUID::invalid;
    }
    s_instance->m_assets[id] = asset;
    s_instance->m_paths[path] = id;
    return id;
  }

  void AssetManager::shutDown()
  {
    delete s_instance;
    s_instance = nullptr;
  }

  template<typename T>
  bool AssetManager::AssetLoader::load(const T&)
  {
    return false;
  }

  template<>
  bool AssetManager::AssetLoader::load(const ARef<Texture2DAsset>& t)
  {
    return AssetLoader::loadTexture2D(t);
  }

  template<>
  bool AssetManager::AssetLoader::load(const ARef<SceneAsset>& t)
  {
    return AssetLoader::loadScene(t);
  }

 template<>
  bool AssetManager::AssetLoader::load(const ARef<ScriptAsset>& t)
  {
    return AssetLoader::loadScript(t);
  }

 template<typename T>
 AssetManager::Asset::Type AssetManager::Asset::getTypeEnum()
 {
   return Type::None;
 }

 template<>
 AssetManager::Asset::Type AssetManager::Asset::getTypeEnum<AssetManager::Texture2DAsset>()
 {
   return Type::Texture2D;
 }
 
 template<>
 AssetManager::Asset::Type AssetManager::Asset::getTypeEnum<AssetManager::SceneAsset>()
 {
   return Type::Scene;
 }
 
 template<>
 AssetManager::Asset::Type AssetManager::Asset::getTypeEnum<AssetManager::ScriptAsset>()
 {
   return Type::Script;
 }

}
