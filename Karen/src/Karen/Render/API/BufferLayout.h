#ifndef BUFFER_LAYOUT_H
#define BUFFER_LAYOUT_H

#include "Karen/Core/Core.h"
#include <string>
#include <stdint.h>

namespace Karen
{
  enum class ShaderDataType : uint8_t
  {
    None = 0,
    Int, Int2, Int3, Int4,
    Float, Float2, Float3, Float4,
    Mat2, Mat3, Mat4,
    Bool
  };

  static std::string ShaderDataTypeToString(ShaderDataType type)
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
    }
    return std::string("Unknown, code: ") + std::to_string((uint32_t)type);
  }

  static uint32_t getTypeSize(ShaderDataType type)
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
    }
    KAREN_CORE_ASSERT(false, "No Matching ShaderDataType To:{0}",ShaderDataTypeToString((type)));
    return 0;
  }

  static uint32_t getComponentCount(ShaderDataType type)
  {
    switch(type)
    {
      case ShaderDataType::None:
        return 0;
      case ShaderDataType::Int:
        return 1;
      case ShaderDataType::Int2:
        return 2;
      case ShaderDataType::Int3:
        return 3;
      case ShaderDataType::Int4:
        return 4;
      case ShaderDataType::Float:
        return 1;
      case ShaderDataType::Float2:
        return 2;
      case ShaderDataType::Float3:
        return 3;
      case ShaderDataType::Float4:
        return 4;
      case ShaderDataType::Mat2:
        return 2 * 2;
      case ShaderDataType::Mat3:
        return 3 * 3;
      case ShaderDataType::Mat4:
        return 4 * 4;
      case ShaderDataType::Bool:
        return 1;
    }
    KAREN_CORE_ASSERT(false, "No Matching ShaderDataType To:{0}", ShaderDataTypeToString(type));
    return 0;
  }

  struct KAREN_API BufferElement
  {
    std::string name = "Element";
    ShaderDataType type = ShaderDataType::None;
    uint32_t offset = 0;
    uint32_t size = 0;
    uint32_t count = 0;
    bool normalized = false;
    BufferElement(const std::string& name, ShaderDataType type, bool normalized = false)
    : name(name), type(type), offset(0), size(getTypeSize(type)),
    count(getComponentCount(type)), normalized(normalized) {}
    BufferElement() {}
  };

  class KAREN_API BufferLayout
  {
  public:
    BufferLayout() {}
    BufferLayout(const std::initializer_list<BufferElement>& elements)
    :m_elements(elements) 
    {
      calcOffsetAndStride();
    }
    inline uint32_t getStride() const {return m_stride;}
    inline const std::vector<BufferElement>& getElements() const 
    {
      return m_elements;
    }

    std::string toString() const
    {
      std::stringstream ss;
      ss << "Stride: " << m_stride;
      for(auto& e : m_elements)
      {
      ss << "name: "<<e.name<<" type: "<<(int)e.type<<" offset: "<< e.offset<<" count: "<<e.count<<" normalized: "<<e.normalized;
      }
      return ss.str();
    }
  private:
    inline void calcOffsetAndStride()
    {
      uint32_t offset = 0;
      for(auto& e : m_elements)
      {
        e.offset = offset;
        offset += e.size;
        m_stride += e.size;
      }
    }
  private:
    std::vector<BufferElement> m_elements;
    uint32_t                   m_stride = 0;
  };

  inline std::ostream& operator<< (std::ostream& os, ShaderDataType type)
  {
    return os << ShaderDataTypeToString(type);
  }
}

#endif //BUFFER_LAYOUT_H
