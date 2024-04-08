#include "Karen/Core/Core.h"
#include "Karen/Core/Math/math.h"
#include "Karen/Render/API/RendererAPI.h"
#include "Karen/Render/API/Shader.h"
#include "Karen/Render/API/VertexArray.h"
#include "Karen/Render/OrthographicCamera.h"


namespace Karen
{
  class KAREN_API Renderer
  {
  public:
    void init();

    void beginScene(const OrthographicCamera& p_camera);
    void submit(const ARef<VertexArray>& p_varray, const ARef<Shader>& p_shader, const Mat4& p_transform = Mat4(1.0f));
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
