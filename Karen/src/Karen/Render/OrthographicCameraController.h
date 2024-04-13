#ifndef ORTHOGERAPHIC_CAMERA_CONTROLLER_H
#define ORTHOGERAPHIC_CAMERA_CONTROLLER_H

#include "Karen/Core/Events/AppEvents.h"
#include "Karen/Core/Events/Event.h"
#include "Karen/Core/Events/MouseEvents.h"
#include "Karen/Core/Timestep.h"
#include "Karen/Render/OrthographicCamera.h"
#include "Karen/Core/ButtonsAndKeyCodes.h"
#include <unordered_map>



namespace Karen
{
  class OrthographicCameraController
  {
  public:
    OrthographicCameraController(float apect_ratio);
    OrthographicCameraController(float left, float right, float bottom, float top);

    
    inline void setZoom(float zoom)
    {
      if(zoom > m_camera.getMaxZoom())
        zoom = m_camera.getMaxZoom();
      else if(zoom < m_camera.getMinZoom())
        zoom = m_camera.getMinZoom();
      else 
        m_zoom = zoom;
    }
    inline void setRotation(float angle)
    {
      m_camera.setRotation(angle);
    }
    inline void zoom(float dzoom)
    {
      m_zoom += dzoom;
      if(m_zoom > m_camera.getMaxZoom())
        m_zoom = m_camera.getMaxZoom();
      else if(m_zoom < m_camera.getMinZoom())
        m_zoom = m_camera.getMinZoom();
    }
    inline void setPos(Vec3 pos)
    {
      m_camera.setPosition(pos);
    }
    inline const Vec3& getPos()
    {
      return m_camera.getPosition();
    }
    inline void rotate(float dangle)
    {
      m_camera.setRotation(m_camera.getRotation() + dangle);
    }
    inline void move(Vec3 dxy)
    {
      m_camera.setPosition(m_camera.getPosition() + dxy);
    }
    inline void enableInternelController(bool enabled)
    {
      m_can_control = enabled;
    }
    inline void enableMouseScrollZoom(bool enabled)
    {
      m_control_zoom_with_mouse_scroll = enabled;
    }
    inline void setRect(float left, float right, float bottom, float top)
    {
      m_camera.setRect(left, right, bottom, top);
    }
    inline OrthographicCamera& getCamera()
    {
      return m_camera;
    }
    void onUpdate(Timestep ts);
    void onEvent(Event& e);
    void setInitial(Vec4 p_initial)
    {
      m_initial = p_initial;
    }
  public:
    enum class CameraDirections : uint8_t
    {
      Up, Down, Left, Right,
      ZoomIn, ZoomOut, RotateClockwise, RotateAntiClockwise
    };


  private:
    bool onMouseScrollEvent(MouseScrolledEvent& e);
    bool onWindowResizeEvent(WindowResizeEvent& e); 
  private:
    float m_aspect_ratio = 1280.0f / 720.0f;
    float m_zoom = 1.0f, m_zoom_speed = 3.0f, m_rotation_speed = 180.0f;
    Vec2 m_speed = { 5.0f, 5.0f };
    Vec4 m_initial = {-1.0f, 1.0f, -1.0f, 1.0f};
    bool m_can_control = true;
    bool m_control_zoom_with_mouse_scroll = true;
    bool m_construct_with_values = false;
    OrthographicCamera m_camera;
  };
}

#endif //ORTHOGERAPHIC_CAMERA_CONTROLLER_H
