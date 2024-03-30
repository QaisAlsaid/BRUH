#include "pch.h"
#include "Render/API/RendererAPI.h"


namespace Karen
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
