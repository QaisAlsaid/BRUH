#include "pch.h"
#include "ImageLoader.h"

namespace Real
{
  ImageLoader::ImageLoader(const std::string& file_path, bool flip)
  {
    bool status = loadFromFile(file_path, flip);
    REAL_CORE_ASSERT_MSG(status, std::string("Failed to load image from file: ") + file_path);
    	std::cout<<"ERROR Loadind Image From File : "<<file_path<<std::endl;
  }

  ImageLoader::~ImageLoader()
  {
    stbi_image_free(m_data);
  }

  bool ImageLoader::loadFromFile(const std::string& file_path, bool flip)
  {
    stbi_set_flip_vertically_on_load(true);
    m_data = stbi_load(file_path.c_str(), &m_width, &m_height, &m_color_channels, 0);
    if(m_data)
    	return true;
    return false;
  }
}
