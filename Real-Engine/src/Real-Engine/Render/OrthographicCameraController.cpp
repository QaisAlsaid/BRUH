#include "Real-Engine/Render/OrthographicCameraController.h"
#include "Real-Engine/Core/ButtonsAndKeyCodes.h"
#include "Real-Engine/Core/Input.h"
#include "Real-Engine/Render/API/RenderCommands.h"


namespace Real
{
  OrthographicCameraController::OrthographicCameraController(const Vec3& p_c, const Vec2& p_s)
  {
    m_aspect_ratio = (float)p_s.x/(float)p_s.y;
    m_camera.setPosition(Vec3(p_c.x - p_s.x/2.0f, p_c.y - p_s.y/2.0f, p_c.z));
    m_initial = Vec4(0, p_s.x, 0.0f,  p_s.y);
    m_initial /= 4.0f;
    m_camera.setRect(m_initial.x, m_initial.y,
      m_initial.z, m_initial.w);
  }

  OrthographicCameraController::OrthographicCameraController(float aspect_ratio, const Vec4& initial)
    : m_aspect_ratio(aspect_ratio), m_initial(initial)
  {
    m_initial /= 4.0f;
     m_camera.setRect(m_aspect_ratio * m_initial.x, m_aspect_ratio * m_initial.y,
      m_initial.z, m_initial.w);
  }
  OrthographicCameraController::OrthographicCameraController(float left, float right, float bottom, float top)
  : m_camera(left/4.0f, right/4.0f, bottom/4.0f, top/4.0f)
  {
    m_initial.x = left;
    m_initial.y = right;
    m_initial.z = bottom;
    m_initial.w = top;
    m_initial /= 4.0f;
  } 
    
  void OrthographicCameraController::onUpdate(Timestep ts)
  {
    if(Input::isKeyPressed(Keyboard::A))
      move(Vec3(-1.0f, 0.0f, 0.0f) * (m_speed.x * ts));
    if(Input::isKeyPressed(Keyboard::D))
      move(Vec3(1.0f, 0.0f, 0.0f) * (m_speed.x * ts));
    if(Input::isKeyPressed(Keyboard::W))
      move(Vec3(0.0f, 1.0f, 0.0f) * (m_speed.x * ts));
    if(Input::isKeyPressed(Keyboard::S))
      move(Vec3(0.0f, -1.0f, 0.0f) * (m_speed.x * ts));
    if(Input::isKeyPressed(Keyboard::E))
      rotate(m_rotation_speed * ts);
    if(Input::isKeyPressed(Keyboard::Q))
      rotate(-m_rotation_speed * ts);
    m_camera.onUpdate(ts);
  }

  void OrthographicCameraController::onViewportResize(uint32_t width, uint32_t height)
  {
    auto e = WindowResizeEvent(width, height);
    onEvent(e);
  }

  void OrthographicCameraController::onEvent(Event& e)
  {
    EventDispatcher dp(e); 
    REAL_CORE_TRACE("Event sent to dispatcher [from Camera controler onEvent()] : {0}", e);
    dp.dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(OrthographicCameraController::onWindowResizeEvent));
    dp.dispatch<MouseScrolledEvent>(BIND_EVENT_FUNCTION(OrthographicCameraController::onMouseScrollEvent));
  }

  bool OrthographicCameraController::onMouseScrollEvent(MouseScrolledEvent& e)
  {
    if(m_control_zoom_with_mouse_scroll)
    {
      zoom(-e.getYoffset() * 0.25);
    }
    return false;
  }
  
  bool OrthographicCameraController::onWindowResizeEvent(WindowResizeEvent& e)
  {
    m_aspect_ratio = (float)e.getWidth()/(float)e.getHeight();
    Vec4 current = m_initial;
    if(m_aspect_ratio > 1.0f)
    {
      current.x *= m_aspect_ratio;
      current.y *= m_aspect_ratio;
    }
    else
    {
      current.z = current.x / m_aspect_ratio;
      current.w = current.y / m_aspect_ratio;
    }
    setRect(current.x, current.y, current.z, current.w);
    RenderCommands::setViewPort(0.0f, 0.0f, e.getWidth(), e.getHeight());
    REAL_CORE_TRACE("ViewPort: ({0}, {1}, {2}, {3}), aspect_ratio: {4}", 0.0f, 0.0f, e.getWidth(), e.getHeight(), m_aspect_ratio);
    return true;
  }
}
