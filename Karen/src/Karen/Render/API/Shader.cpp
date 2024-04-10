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
        KAREN_CORE_ASSERT(false, "Renderer API: 'NONE' Not suported at the moment");
        return nullptr;
      case RendererAPI::API::OpenGl:
        return std::make_shared<OpenGlShader>(vp, fp);
    }
    KAREN_CORE_ASSERT(false, "Unknown Renderer API");
    return nullptr;
  }
  
  ARef<Shader> Shader::create()
  {
    switch(RendererAPI::getAPI())
    {
      case RendererAPI::API::NONE:
        KAREN_CORE_ASSERT(false, "Render API: 'NONE' Not suported at the moment");
        return nullptr;
      case RendererAPI::API::OpenGl:
        return std::make_shared<OpenGlShader>();
    }
    KAREN_CORE_ASSERT(false, "Unknown RendererAPI");
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
    return std::string("Unknown, code: ") + std::to_string((uint32_t)type);
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
    KAREN_CORE_ASSERT(false, "No Matching ShaderDataType To:{0}",ShaderDataTypeToString((type)));
    return 0;
  }

  uint32_t hashType(const std::string & tn)
  {
    uint32_t t_id = 0;
    for(auto ch : tn)
    {
      t_id += ch;
    }
    return t_id;
  }

  ShaderDataType getTypeFromHash(uint32_t h)
  {
    switch(h)
    {
      case ('i' + 'n' + 't'):
        return ShaderDataType::Int;
      case ('f' + 'l' + 'o' + 't'):
        return ShaderDataType::Float;
      case ('v' + 'e' + 'c' + '2'):
        return ShaderDataType::Float2;
      case ('v' + 'e' + 'c' + '3'):
        return ShaderDataType::Float3;
      case ('v' + 'e' + 'c' + '4'):
        return ShaderDataType::Float4;
      case ('m' + 'a' + 't' + '2'):
        return ShaderDataType::Mat2;
      case ('m' + 'a' + 't' + '3'):
        return ShaderDataType::Mat3;
      case ('m' + 'a' + 't' + '4'):
        return ShaderDataType::Mat4;
      case ('s' + 'a' + 'm' + 'p' + 'l' + 'e' + 'r' + '2' + 'D'):
        return ShaderDataType::Sampler2D;
    }
    KAREN_CORE_ASSERT(false, "Unknown ShaderDataType Hash");
    return ShaderDataType::None;
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
    KAREN_CORE_ASSERT(false, "Not a valid ShaderType");
    return "";
  }
}
