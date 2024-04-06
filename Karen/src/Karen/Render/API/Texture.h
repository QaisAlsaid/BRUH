#ifndef TEXTURE_H
#define TEXTURE_H

#include "Karen/Core/Core.h"


namespace Karen
{
  class KAREN_API Texture
  {
  public:
    virtual ~Texture() = default;

    virtual uint32_t getWidth()  const = 0;
    virtual uint32_t getHeight() const = 0;

    virtual void bind(uint8_t slot = 0) const = 0;
  };


  class KAREN_API Texture2D : public Texture
  {
  public:
    static ARef<Texture2D> create(const std::string& file_path, bool flip_v = true);
  };
}


#endif //TEXTURE_H
