#include "pch.h"
#include "Real-Engine/Render/API/RendererAPI.h"


namespace Real
{
  RendererAPI::API RendererAPI::s_API = RendererAPI::API::NONE;
  void RendererAPI::setAPI(API p_API)
  {
    s_API = p_API;
  }

  RendererAPI::API RendererAPI::getAPI()
  {
    return s_API;
  } 
}
