#include "pch.h"
#include "Real-Engine/Render/API/Texture.h"
#include "Real-Engine/Render/API/RendererAPI.h"
#include "Platforms/OpenGl/OpenGlTexture.h"

namespace Real
{
  ARef<Texture2D> Texture2D::create(const std::string& file_path, bool flip_v, Filters fls)
  {
    switch(RendererAPI::getAPI())
    {
      case RendererAPI::API::NONE:
        REAL_CORE_ASSERT_MSG(false, "Render API None not supported at the moment");
      case RendererAPI::API::OpenGl:
        return Real::createARef<OpenGlTexture2D>(file_path, flip_v, fls);
    }
    REAL_CORE_ASSERT_MSG(false, "Unknown API");
    return nullptr;
  }


  ARef<Texture2D> Texture2D::create(uint32_t height, uint32_t width, size_t data_size, const void* data)
  {
    switch(RendererAPI::getAPI())
    {
      case RendererAPI::API::NONE:
        REAL_CORE_ASSERT_MSG(false, "Render API None not supported at the moment");
      case RendererAPI::API::OpenGl:
        return Real::createARef<OpenGlTexture2D>(height, width, data_size, data);
    }
    REAL_CORE_ASSERT_MSG(false, "Unknown API");
    return nullptr;

  }

  bool isEqual(const ARef<Texture>& lhs, const ARef<Texture>& rhs)
  {
    return *lhs.get() == *rhs.get();
  }
}
