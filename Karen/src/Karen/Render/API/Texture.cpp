#include "pch.h"
#include "Karen/Render/API/Texture.h"
#include "Karen/Render/API/RendererAPI.h"
#include "Platforms/OpenGl/OpenGlTexture.h"

namespace Karen
{
  ARef<Texture2D> Texture2D::create(const std::string& file_path, bool flip_v)
  {
    switch(RendererAPI::getAPI())
    {
      case RendererAPI::API::NONE:
        KAREN_CORE_ASSERT_MSG(false, "Render API None not supported at the moment");
      case RendererAPI::API::OpenGl:
        return Karen::createARef<OpenGlTexture2D>(file_path, flip_v);
    }
    KAREN_CORE_ASSERT_MSG(false, "Unknown API");
    return nullptr;
  }


  ARef<Texture2D> Texture2D::create(uint32_t height, uint32_t width, size_t data_size, const void* data)
  {
    switch(RendererAPI::getAPI())
    {
      case RendererAPI::API::NONE:
        KAREN_CORE_ASSERT_MSG(false, "Render API None not supported at the moment");
      case RendererAPI::API::OpenGl:
        return Karen::createARef<OpenGlTexture2D>(height, width, data_size, data);
    }
    KAREN_CORE_ASSERT_MSG(false, "Unknown API");
    return nullptr;

  }
}
