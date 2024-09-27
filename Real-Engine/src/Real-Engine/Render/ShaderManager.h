#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <unordered_map>
#include "Real-Engine/Core/Core.h"
#include "Real-Engine/Render/API/Shader.h"


namespace Real
{
  class REAL_API ShaderManager
  {
  public:
    ShaderManager() = default;
    ShaderManager(const std::string& config_file_path);
    void LoadConfig(const std::string& config_file_path);
    void Add(const std::string& name, const ARef<Shader>& shader); 
    void Add(const std::string& path, const std::string& name); 
    ARef<Shader> get(const std::string& name);
    const std::unordered_map<std::string, ARef<Shader>>& get() const;
  private:
    std::unordered_map<std::string, ARef<Shader>> m_shaders;
  };
}
#endif //SHADER_MANAGER_H
