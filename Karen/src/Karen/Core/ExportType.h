#ifndef EXPORT_TYPE_H
#define EXPORT_TYPE_H

#include "Core.h"
#include "Karen/Core/Math/math.h"

namespace Karen
{
  class KAREN_API ExportType 
  {
  public:
    enum class Type 
    {
      None = 0, Float, Int, String, Vec4, Vec3, Vec2, RGBA_Color
    };
    ExportType(Type T, void* data) 
      :m_data_type(T), m_raw_data(data) {} ;

    inline Type getType()     const { return m_data_type; }
    inline void* getRawData() const { return m_raw_data;  }

    template<typename T>
    T* getAs() const { return static_cast<T*>(m_raw_data); }
  private:
    Type  m_data_type = Type::None;
    void* m_raw_data = nullptr;
  };
}


#endif //EXPORT_TYPE_H
