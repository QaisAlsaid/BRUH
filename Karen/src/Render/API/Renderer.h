#include "Karen/Core.h"
#include "Render/API/RendererAPI.h"
#include "Render/API/Shader.h"
#include "Render/API/VertexArray.h"
#include "Render/OrthographicCamera.h"


namespace Karen
{
  class KAREN_API Renderer
  {
  public:
    void beginScene(const OrthographicCamera& p_camera);
    void submit(const ARef<VertexArray>& p_varray, const ARef<Shader>& p_shader);
    void endScene();
    inline static RendererAPI::API getAPI()
    {
      return RendererAPI::getAPI();
    }

    inline static void setRenderAPI(RendererAPI::API p_API)
    {
      RendererAPI::setAPI(p_API);
    }

  private:
    struct sceneData
    {
      Mat4 proj_view;
    };
    sceneData m_scene_data;
  };
}
