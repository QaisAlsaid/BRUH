#ifndef SHADER_H
#define SHADER_H

#include "Karen/Core/Core.h"
#include "Karen/Core/Math/math.h"
#include <string>

namespace Karen
{

  enum class ShaderDataType : uint8_t
  {
    None = 0,
    Int, Int2, Int3, Int4,
    Float, Float2, Float3, Float4,
    Mat2, Mat3, Mat4,
    Sampler2D, Bool
  };

  enum class ShaderType : uint8_t
  {
    None = 0, Vertex, Fragment
  };

  struct UniformData
  {
    uint32_t location = 0;
    ShaderDataType type;
  };

  std::string ShaderDataTypeToString(ShaderDataType type);
  uint32_t getTypeSize(ShaderDataType type); 
  uint32_t hashType(const std::string& type_name);
  ShaderDataType getTypeFromHash(uint32_t hash);
  std::string ShaderTypetoString(ShaderType type);


  class KAREN_API Shader
  {
  public:
    virtual ~Shader() {}

    virtual void bind()   const = 0;
    virtual void unbind() const = 0;
    virtual void loadFromFile(const std::string& vp, const std::string& fp) = 0;
    virtual void setUniform(const std::string& name, const Mat4& value) = 0;
    virtual void setUniform(const std::string& name, const Vec2& value) = 0;
    virtual void setUniform(const std::string& name, int value) = 0;
    virtual void setUniform(const std::string& name, const Vec4& value) = 0;

    static ARef<Shader> create();
    static ARef<Shader> create(const std::string& vp, const std::string& fp);
  };
}

#endif //SHADER_H
