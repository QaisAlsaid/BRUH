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
    static void init();
    static void onWindowResize(uint32_t left, uint32_t bottom, uint32_t right, uint32_t top);
    static void beginScene(const OrthographicCamera& p_camera);
    static void submit(const ARef<VertexArray>& p_varray, const ARef<Shader>& p_shader, const Mat4& p_transform = Mat4(1.0f));
    static void endScene();
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
    static sceneData* s_scene_data;
  };
}
