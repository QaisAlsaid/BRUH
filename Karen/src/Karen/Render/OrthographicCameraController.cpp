#include "Karen/Render/OrthographicCameraController.h"
#include "Karen/Core/ButtonsAndKeyCodes.h"
#include "Karen/Core/Events/AppEvents.h"
#include "Karen/Core/Events/Event.h"
#include "Karen/Core/Events/MouseEvents.h"
#include "Karen/Core/Input.h"
#include "Karen/Render/API/RenderCommands.h"


namespace Karen
{
  OrthographicCameraController::OrthographicCameraController(float aspect_ratio)
    : m_aspect_ratio(aspect_ratio), m_camera(-m_aspect_ratio * m_zoom, m_aspect_ratio * m_zoom,
      -m_zoom, m_zoom)
  {
    m_construct_with_values = false;
  }
  OrthographicCameraController::OrthographicCameraController(float left, float right, float bottom, float top)
  : m_camera(left, right, bottom, top)
  {
    m_construct_with_values = true;
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
    if(Input::isKeyPressed(Keyboard::Up))
      zoom(m_zoom_speed * ts);
    if(Input::isKeyPressed(Keyboard::Down))
      zoom(-m_zoom_speed * ts);
    if(Input::isKeyPressed(Keyboard::E))
      rotate(m_rotation_speed * ts);
    if(Input::isKeyPressed(Keyboard::Q))
      rotate(-m_rotation_speed * ts);
    m_camera.setZoom(m_zoom);
    m_camera.onUpdate(ts);
  }
  void OrthographicCameraController::onEvent(Event& e)
  {
    EventDispatcher dp(e); 
    KAREN_CORE_INFO("Event sent to dispatcer [from Camera controler onEvent()] : {0}", e);
    dp.dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(OrthographicCameraController::onWindowResizeEvent));
    dp.dispatch<MouseScrolledEvent>(BIND_EVENT_FUNCTION(OrthographicCameraController::onMouseScrollEvent));
  }

  bool OrthographicCameraController::onMouseScrollEvent(MouseScrolledEvent& e)
  {
    if(m_control_zoom_with_mouse_scroll)
    {
      zoom(-e.getYoffset());
    }
    return false;
  }
  
  bool OrthographicCameraController::onWindowResizeEvent(WindowResizeEvent& e)
  {
    if(!m_construct_with_values)
    {
      m_aspect_ratio = (float)e.getWidth()/(float)e.getHeight();
      setRect(-m_aspect_ratio * m_zoom, m_aspect_ratio * m_zoom, -m_zoom, m_zoom);
    }
    else 
    {
      setRect(0, 0, e.getWidth(), e.getHeight());
    }
    RenderCommands::setViewPort(0.0f, 0.0f, e.getWidth(), e.getHeight());
    KAREN_CORE_INFO("ViewPort: ({0}, {1}, {2}, {3}), aspect_ratio: {4}", 0.0f, 0.0f, e.getWidth(), e.getHeight(), m_aspect_ratio);
    return false;
  }

}
