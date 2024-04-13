#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "pch.h"
#include "Karen/Render/OrthographicCamera.h"


namespace Karen
{
  OrthographicCamera::OrthographicCamera()
  {
  }

  OrthographicCamera::OrthographicCamera(float p_left, float p_right, float p_bottom, float p_top, Vec3 p_pos)
    : m_left(p_left), m_right(p_right),
    m_bottom(p_bottom), m_top(p_top),
    m_pos(p_pos)
  {
  }

  void OrthographicCamera::setRect(float p_left, float p_right, float p_bottom, float p_top)
  {
    m_left   = p_left;
    m_right  = p_right;
    m_bottom = p_bottom;
    m_top    = p_top;
  }

  void OrthographicCamera::onUpdate(float delta)
  {
    m_proj = glm::ortho(m_left * 1/m_zoom, m_right * 1/m_zoom, m_bottom * 1/m_zoom, m_top * 1/m_zoom, -1.0f, 1.0f);
    m_view = Mat4(1.0f); 
    m_view = glm::translate(m_view, -m_pos);
    m_view = glm::rotate(m_view, glm::radians(m_rotation), Vec3(0, 0, 1)); 
    m_pv = m_proj * m_view;
  }
}
