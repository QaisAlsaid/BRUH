#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <stb_image.h>
#include <string>
#include <memory>
#include "Karen/Core/Core.h"

namespace Karen
{
  class ImageLoader
  {
  public:
    ImageLoader(const std::string& file_path, bool set_vertical_on_load_flip = false);
    ImageLoader() = default;
    ~ImageLoader();
    
    [[nodiscard]] bool loadFromFile(const std::string& file_path, bool set_vertical_on_load_flip = true);
    
    inline uint32_t getWidth() const
    {
	    return m_width;
    }
    inline uint32_t getHeight() const
    {
	    return m_height;
    }
    inline uint32_t getColorChannels() const
    {
	    return m_color_channels;
    }
    unsigned char* getImageData() const
    {
	    return m_data;
    }
  private:
    int            m_width          = 0;
    int            m_height         = 0;
    int            m_color_channels = 0;
    unsigned char* m_data           = nullptr;
  };
}
#endif //IMAGELOADER_H
