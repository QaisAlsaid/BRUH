#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include "Karen/Render/API/Texture.h"
#include "Karen/Core/CommanUtils/ImageLoader.h"


namespace Karen
{
  class KAREN_API OpenGlTexture2D : public Texture2D
  {
  public:
    OpenGlTexture2D(const std::string& file_path, bool flip_v = true);
    OpenGlTexture2D(uint32_t width, uint32_t height, size_t data_size, const void* data);
    ~OpenGlTexture2D();
    void bind(uint8_t slot = 0) const override;

    void setData(size_t data_size, const void* data) override;

    uint32_t getWidth()  const override 
    {
      return m_width;
    }
    uint32_t getHeight() const override
    {
      return m_height;
    }
  private:
    uint32_t m_width  = 0;
    uint32_t m_height = 0;
    uint32_t m_renderer_id = 0;
    uint32_t m_data_format, m_internal_format;
    ImageLoader m_image;
  };
}
#endif //OPENGL_TEXTURE_H
