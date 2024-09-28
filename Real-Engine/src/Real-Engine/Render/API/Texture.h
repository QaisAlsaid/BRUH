#ifndef TEXTURE_H
#define TEXTURE_H

#include "Real-Engine/Core/Core.h"


namespace Real 
{
  class REAL_API Texture
  {
  public:
    enum class FilterType
    {
      Linear, Nearest
    };
    enum class WrapMode
    {
      ClampToEdge, ClampToBorder, Repeat
    };
    struct Filters
    {
      Filters() = default;
      Filters(FilterType min, FilterType mag, WrapMode s, WrapMode r, WrapMode t)
        : min_filter(min), mag_filter(mag), wrap_s(s), wrap_r(r), wrap_t(t) {}
      FilterType min_filter = FilterType::Nearest;
      FilterType mag_filter = FilterType::Nearest;
      
      WrapMode wrap_s = WrapMode::ClampToEdge;
      WrapMode wrap_r = WrapMode::ClampToEdge;
      WrapMode wrap_t = WrapMode::ClampToEdge;
    };
  public:
    virtual ~Texture() = default;

    virtual uint32_t getWidth()  const = 0;
    virtual uint32_t getHeight() const = 0;
    
    virtual uint32_t getRendererID() const = 0;
    virtual void setData(size_t data_size, const void* data) = 0;
    virtual void bind(uint8_t slot = 0) const = 0;
    virtual bool operator==(const Texture& rhs) const = 0;
  };


  class REAL_API Texture2D : public Texture
  {
  public:
    static ARef<Texture2D> create(const std::string& file_path, bool flip_v = true, Filters = Filters());
    static ARef<Texture2D> create(uint32_t height = 1, uint32_t width = 1, size_t data_size = 0, const void* data = nullptr);
  };

  bool isEqual(const ARef<Texture>& lhs, const ARef<Texture>& rhs);
 }


#endif //TEXTURE_H
