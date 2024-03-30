#include "Karen/Core.h"
#include "Render/API/RendererAPI.h"
#include "Render/API/VertexArray.h"


namespace Karen
{
  class KAREN_API Renderer
  {
  public:
    void beginScene();
    void submit(const ARef<VertexArray>& p_varray);
    void endScene();
    inline static RendererAPI::API getAPI()
    {
      return RendererAPI::getAPI();
    }

    inline static void setRenderAPI(RendererAPI::API p_API)
    {
      RendererAPI::setAPI(p_API);
    }
  };
}
