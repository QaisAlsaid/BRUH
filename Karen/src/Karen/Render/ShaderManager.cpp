#include "Karen/Render/ShaderManager.h"
#include "Karen/Core/Log.h"

#include "Karen/Render/API/Shader.h"
#include "pugixml.hpp"


namespace Karen
{
  static std::string getNameFromPath(const std::string& path);
  ShaderManager::ShaderManager(const std::string& p_config_file_path)
  {
    LoadConfig(p_config_file_path);
  }

  void ShaderManager::LoadConfig(const std::string& p_config_file_path)
  {

    KAREN_CORE_INFO("Started Loading Shaders from config");
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(p_config_file_path.c_str(),
    pugi::parse_default|pugi::parse_declaration);
    if (!result)
    {
      KAREN_CORE_ERROR("Parse Error at: {0}, offset: {1}",result.description(), result.offset);
      return;
    }
    pugi::xml_node list = doc.child("ShadersList");
    uint32_t index = 0;
    for (pugi::xml_node shader : list.children())
    {
      ++index;
      bool is_path = true;
      auto path = shader.child("Path");
      std::string name = shader.child("Name").child_value();
      std::string vert_path = path.child_value("VertPath");
      std::string frag_path = path.child_value("FragPath");

      if(!path )
      {
        is_path = false;
        KAREN_CORE_ERROR("No Path for Shader number: {0} name: {1} it Won't be loaded", index, name);
      }
      else
      {
        if(vert_path == "")
        {
          KAREN_CORE_ERROR("No VertPath specified for Shader number: {0} name: {1}", index, name);
          is_path = false;
        }
        if(frag_path == "")
        {
          KAREN_CORE_ERROR("No FragPath specified for Shader number: {0} name: {1}", index, name);
          is_path = false;
        }
      }
      if(name == "" || (!shader.child("Name")))
      {
        if(is_path)
        {
          std::string path_name = getNameFromPath(vert_path);
          KAREN_CORE_WARN("No name specified for Shader number: {0} VertPath : {1}, fragPath: {2} , will be assigned the name : {1}", index, vert_path, frag_path);
          name = path_name;
        }
      }
      KAREN_CORE_INFO("Shader number: {0} Name: {1} VertPath: {2} FragPath: {3}", index, name, vert_path, frag_path);
      if(is_path)
      {
        ARef<Shader> sh = Shader::create(vert_path, frag_path);
        m_shaders[name] = sh; 
      }
      else
      {
        ARef<Shader> sh = Shader::create();
        if(name == "")
        {
          name = "Shader" + std::to_string(index);
        }
        KAREN_CORE_WARN("No path specified for Shader number: {0} it will be assigned the name: {1}", index, name);
        m_shaders[name] = sh;
      }
      KAREN_CORE_INFO("NAME : {0}", name);
    }
  }

  void ShaderManager::Add(const std::string& p_name, const ARef<Shader>& p_shader)
  {
    KAREN_CORE_ASSERT_MSG(m_shaders.find(p_name) != m_shaders.end(), std::string("Shader with Name: {0} already exist it won't be added"));
    m_shaders[p_name] = p_shader;
  }

  void ShaderManager::Add(const std::string& p_vert_path, const std::string& p_frag_path, const std::string& p_name) 
  {
    std::string new_name = p_name;
    if(p_vert_path == "" || p_frag_path == "")
    {
      KAREN_CORE_ASSERT_MSG(false,"No Path specified Shader Won't be Added");
    }
    else if(p_name == "")
    {
      new_name = getNameFromPath(p_vert_path);
      KAREN_CORE_WARN("No Name specified for Shader VertPath: {0} FragPath: {1} it will be assigned the name: {2}", p_vert_path, p_frag_path, new_name); 
    }
    ARef<Shader> shader = Shader::create(p_vert_path, p_frag_path);
    KAREN_CORE_ASSERT_MSG(m_shaders.find(new_name) != m_shaders.end(), std::string("Shader with Name: {0} already exist it wont be added" + new_name))
    m_shaders[new_name] = shader;
  }

  ARef<Shader> ShaderManager::get(const std::string& p_name)
  {
    if(m_shaders.find(p_name) == m_shaders.end())
    {
      KAREN_CORE_ASSERT_MSG(false, std::string("Shader with name: ") + p_name + "Not found");
      return nullptr;
    }
    else
      return m_shaders.at(p_name);
  }
  const std::unordered_map<std::string, ARef<Shader>>& ShaderManager::get() const
  {
    return m_shaders;
  }

  static std::string getNameFromPath(const std::string& path)
  {
    auto last_dir_sep = path.find_last_of("\\/");
    last_dir_sep = last_dir_sep == std::string::npos ? 0 : last_dir_sep + 1;
    auto last_dot = path.find_last_of(".");
    auto count = last_dot == std::string::npos ? path.size() - last_dir_sep : last_dot - last_dir_sep;
    auto new_name = path.substr(last_dir_sep, count);
    return new_name;
  }
}


