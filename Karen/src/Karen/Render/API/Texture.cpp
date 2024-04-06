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
        KAREN_CORE_ASSERT(false, "Render API None not supported at the moment");
      case RendererAPI::API::OpenGl:
        return std::make_shared<OpenGlTexture2D>(file_path, flip_v);
    }
    KAREN_CORE_ASSERT(false, "Unknown API");
    return nullptr;
  }
}
