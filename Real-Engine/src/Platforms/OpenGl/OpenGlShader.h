#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include "Real-Engine/Render/API/Shader.h"

namespace Real
{
  class OpenGlShader : public Shader
  {
  public:
    OpenGlShader();
    OpenGlShader(const std::string& p);
    ~OpenGlShader();

    void bind()   const override;
    void unbind() const override;
    //TODO: make it bool
    void loadFromFile(const std::string& path) override;
    void setUniform(const std::string& name, const Mat4& value) override;
    void setUniform(const std::string& name, int value) override;
    void setUniform(const std::string& name, const Vec4& value) override;
    void setUniform(const std::string& name, const Vec2& value) override;
    void setUniform(const std::string& name, const int* value, uint32_t count) override;
  private:
    bool compileShaders(const std::string& vs, const std::string& fs);
    bool createProgram();
    void cacheUniforms(std::string vs, std::string fs);
    void findUniformsAndData(const std::vector<std::string>& tokens);
    bool preprocessShader(const std::string& file_path, std::string& vs, std::string& fs);
  private:
    std::string vertex_src;
    std::string fragment_src;
    std::unordered_map<std::string, UniformData> m_uniforms;
    uint32_t    m_program_id, m_vs_id, m_fs_id;
  };
}
#endif //OPENGL_SHADER_H
