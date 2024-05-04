#include "SceneCamera.h"
#include "Karen/Render/API/RenderCommands.h"


namespace Karen 
{
  SceneCamera::SceneCamera(const OrthographicData& data)
    : m_ortho(data)
  {
    m_type = ProjectionType::Orthographic;
    reCalculate();
  }

  SceneCamera::SceneCamera(const PerspectiveData& data)
    : m_persp(data)
  {
    m_type = ProjectionType::Perspective;
    reCalculate();
  }

  void SceneCamera::reCalculate()
  {
    switch(m_type)
    {
      case ProjectionType::Orthographic:
      {
        m_projection = glm::ortho(-m_ortho.size/2.0f * m_zoom_out_level * m_aspect_ratio,
            m_ortho.size/2.0f * m_zoom_out_level * m_aspect_ratio,
           -m_ortho.size/2.0f * m_zoom_out_level,
            m_ortho.size/2.0f * m_zoom_out_level,
           -1.0f, 1.0f);
        break;
      }
      case ProjectionType::Perspective:
      {
        m_projection = glm::perspective(m_persp.fov, m_aspect_ratio, m_persp.near_clip, m_persp.far_clip);
        break;
      }
    }
  }

  void SceneCamera::setOrthographicData(const OrthographicData& data)
  {
    m_ortho = data;
    reCalculate();
  }

  void SceneCamera::setPerspectiveData(const PerspectiveData& data)
  {
    m_persp = data;
    reCalculate();
  }

  void SceneCamera::setOrthographicData(float size, float nc, float fc)
  {
    m_ortho = {size, nc, fc};
    reCalculate();
  }

  void SceneCamera::setPerspectiveData(float fov, float nc, float fc)
  {
    m_persp = {fov, nc, fc};
  }

  void SceneCamera::setViewport(uint32_t width, uint32_t height)
  {
    m_aspect_ratio = (float)width/(float)height;
    reCalculate();
    RenderCommands::setViewPort(0, 0, width, height);
  }

}
