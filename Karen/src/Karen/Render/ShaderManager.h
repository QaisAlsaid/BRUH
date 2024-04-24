#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <unordered_map>
#include "Karen/Core/Core.h"
#include "Karen/Render/API/Shader.h"


namespace Karen
{
  class ShaderManager
  {
  public:
    ShaderManager() = default;
    ShaderManager(const std::string& config_file_path);
    void LoadConfig(const std::string& config_file_path);
    void Add(const std::string& name, const ARef<Shader>& shader); 
    void Add(const std::string& vert_path,const std::string& name); 
    ARef<Shader> get(const std::string& name);
    const std::unordered_map<std::string, ARef<Shader>>& get() const;
  private:
    std::unordered_map<std::string, ARef<Shader>> m_shaders;
  };
}
#endif //SHADER_MANAGER_H
