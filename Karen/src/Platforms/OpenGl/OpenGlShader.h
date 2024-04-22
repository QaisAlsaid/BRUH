#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include "Karen/Render/API/Shader.h"

namespace Karen
{
  class OpenGlShader : public Shader
  {
  public:
    OpenGlShader();
    OpenGlShader(const std::string& vp, const std::string& fp);
    ~OpenGlShader();

    void bind()   const override;
    void unbind() const override;
    //TODO: make it bool
    void loadFromFile(const std::string& vp, const std::string& fp) override;
    void setUniform(const std::string& name, const Mat4& value) override;
    void setUniform(const std::string& name, int value) override;
    void setUniform(const std::string& name, const Vec4& value) override;
    void setUniform(const std::string& name, const Vec2& value) override;
    void setUniform(const std::string& name, const int* value, uint32_t count) override;
  private:
    bool compileShaders(std::string& vs, std::string& fs);
    bool createProgram();
    void cacheUniforms(std::string& vs, std::string& fs);
    void findUniformsAndData(const std::vector<std::string>& tokens);
  private:
    std::string vertex_src;
    std::string fragment_src;
    std::unordered_map<std::string, UniformData> m_uniforms;
    uint32_t    m_program_id, m_vs_id, m_fs_id;
  };
}
#endif //OPENGL_SHADER_H
