#include "SceneCamera.h"
#include "Karen/Render/API/RenderCommands.h"


namespace Karen 
{
  SceneCamera::SceneCamera(Vec2 size)
    : m_size(size)
  {
    reCalculate();
  }

  void SceneCamera::reCalculate()
  {
    m_projection = glm::ortho(0.0f, m_size.x * m_zoom_out_level * m_aspect_ratio,
      0.0f, m_size.y * m_zoom_out_level, -1.0f, 1.0f);
  }

  void SceneCamera::setOrtho(Vec2 size)
  {
    m_size = size;
    m_aspect_ratio = size.x / size.y;
    reCalculate();
  }

  void SceneCamera::setViewport(uint32_t width, uint32_t height)
  {
      m_aspect_ratio = (float)width/(float)height;
      reCalculate();
      RenderCommands::setViewPort(0, 0, width, height);
  }

}
