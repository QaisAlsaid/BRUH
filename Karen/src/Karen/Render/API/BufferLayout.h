#ifndef BUFFER_LAYOUT_H
#define BUFFER_LAYOUT_H

#include "Karen/Core/Core.h"
#include "Karen/Render/API/Shader.h"
#include <string>
#include <stdint.h>

namespace Karen
{ 
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
