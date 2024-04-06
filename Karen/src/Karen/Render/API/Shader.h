#ifndef SHADER_H
#define SHADER_H

#include "Karen/Core/Core.h"
#include "Karen/Core/Math/math.h"
#include <string>

namespace Karen
{
  enum class ShaderType : uint8_t
  {
    None = 0, Vertex, Fragment
  };


  class KAREN_API Shader
  {
  public:
    virtual ~Shader() {}

    virtual void bind()   const = 0;
    virtual void unbind() const = 0;
    virtual void loadFromFile(const std::string& vp, const std::string& fp) = 0;
    virtual void setUniformMat4fv(const std::string& name, const Mat4& value) = 0;
    virtual void setUniformInt(const std::string& name, int value) = 0;

    static Shader* create();
    static Shader* create(const std::string& vp, const std::string& fp);
  };
}

#endif //SHADER_H
