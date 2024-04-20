#include "Karen/Core/Assertion.h"
#include "pch.h"
#include "Karen/Render/API/Shader.h"
#include "Karen/Render/API/RendererAPI.h"
#include "Platforms/OpenGl/OpenGlShader.h"


namespace Karen
{
  ARef<Shader> Shader::create(const std::string& vp, const std::string& fp)
  {
    switch(RendererAPI::getAPI())
    {
      case RendererAPI::API::NONE:
        KAREN_CORE_ASSERT_MSG(false, "Renderer API: 'NONE' Not suported at the moment");
        return nullptr;
      case RendererAPI::API::OpenGl:
        return Karen::createARef<OpenGlShader>(vp, fp);
    }
    KAREN_CORE_ASSERT_MSG(false, "Unknown Renderer API");
    return nullptr;
  }
  
  ARef<Shader> Shader::create()
  {
    switch(RendererAPI::getAPI())
    {
      case RendererAPI::API::NONE:
        KAREN_CORE_ASSERT_MSG(false, "Render API: 'NONE' Not suported at the moment");
        return nullptr;
      case RendererAPI::API::OpenGl:
        return Karen::createARef<OpenGlShader>();
    }
    KAREN_CORE_ASSERT_MSG(false, "Unknown RendererAPI");
    return nullptr;
  }


  std::string ShaderDataTypeToString(ShaderDataType type)
  {
    switch(type)
    {
      case ShaderDataType::None:
          return "None";
        case ShaderDataType::Int:
          return "Int";
        case ShaderDataType::Int2:
          return "Int2";
        case ShaderDataType::Int3:
          return "Int3";
        case ShaderDataType::Int4:
          return "Int4";
        case ShaderDataType::Float:
          return "Float1";
        case ShaderDataType::Float2:
          return "Float2";
        case ShaderDataType::Float3:
          return "Float3";
        case ShaderDataType::Float4:
          return "Float4";
        case ShaderDataType::Mat2:
          return "Mat2";
        case ShaderDataType::Mat3:
          return "Mat3";
        case ShaderDataType::Mat4:
          return "Mat4";
        case ShaderDataType::Bool:
          return "Bool";
        case ShaderDataType::Sampler2D:
          return "Sampler2D";
    }
    KAREN_CORE_ASSERT_MSG("Unknown, code: {0}", (uint32_t)type);
    return "";
  }

  uint32_t getTypeSize(ShaderDataType type)
  {
    switch(type)
    {
      case ShaderDataType::None:
        return 0;
      case ShaderDataType::Int:
        return sizeof(int);
      case ShaderDataType::Int2:
        return sizeof(int) * 2;
      case ShaderDataType::Int3:
        return sizeof(int) * 3;
      case ShaderDataType::Int4:
        return sizeof(int) * 4;
      case ShaderDataType::Float:
        return sizeof(float);
      case ShaderDataType::Float2:
        return sizeof(float) * 2;
      case ShaderDataType::Float3:
        return sizeof(float) * 3;
      case ShaderDataType::Float4:
        return sizeof(float) * 4;
      case ShaderDataType::Mat2:
        return sizeof(float) * 2 * 2;
      case ShaderDataType::Mat3:
        return sizeof(float) * 3 * 3;
      case ShaderDataType::Mat4:
        return sizeof(float) * 4 * 4;
      case ShaderDataType::Bool:
        return sizeof(bool);
      case ShaderDataType::Sampler2D:
        return sizeof(int);
    }
    KAREN_CORE_ASSERT_MSG(false, "No Matching ShaderDataType To:{0}",ShaderDataTypeToString((type)));
    return 0;
  }

  ShaderDataType getTypeFromString(const std::string&  p_tn)
  {
    if(p_tn == "int")
      return ShaderDataType::Int;
    else if(p_tn == "float")
      return ShaderDataType::Float;
    else if(p_tn == "vec2")
      return ShaderDataType::Float2;
    else if(p_tn == "vec3")
      return ShaderDataType::Float3;
    else if(p_tn == "vec4")
      return ShaderDataType::Float4;
    else if(p_tn == "mat2")
      return ShaderDataType::Mat2;
    else if(p_tn == "mat3")
      return ShaderDataType::Mat3;
    else if(p_tn == "mat4")
      return ShaderDataType::Mat4;
    else if(p_tn == "sampler2D")
      return ShaderDataType::Sampler2D;
    else if(p_tn == "bool")
      return ShaderDataType::Bool;
    else
    {
      KAREN_CORE_ASSERT_MSG(false, "Unknown ShaderDataType {0}", p_tn);
      return ShaderDataType::None;
    }
  }

  std::string ShaderTypetoString(ShaderType p_t)
  {
    switch(p_t)
    {
      case ShaderType::None:
        return "None";
      case ShaderType::Vertex:
        return "Vertex";
      case ShaderType::Fragment:
        return "Fragment";
    }
    KAREN_CORE_ASSERT_MSG(false, "Not a valid ShaderType");
    return "";
  }
}
