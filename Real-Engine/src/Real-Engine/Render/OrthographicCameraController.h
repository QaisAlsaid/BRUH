#ifndef ORTHOGERAPHIC_CAMERA_CONTROLLER_H
#define ORTHOGERAPHIC_CAMERA_CONTROLLER_H

#include "Real-Engine/Core/Events/Events.h"
#include "Real-Engine/Core/Timestep.h"
#include "Real-Engine/Render/OrthographicCamera.h"
#include <unordered_map>



namespace Real
{
  class OrthographicCameraController
  {
  public:
    OrthographicCameraController(const Vec3& center, const Vec2& size);
    OrthographicCameraController(float apect_ratio, const Vec4& initial = Vec4(-1.0f, 1.0f, -1.0f, 1.0f));
    OrthographicCameraController(float left, float right, float bottom, float top);

    
    inline void setRotation(float angle)
    {
      m_camera.setRotation(angle);
    }
    inline void zoom(float dzoom)
    {
      m_camera.setZoom(m_camera.getZoom() + dzoom);
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
    inline void setSpeed(const Vec2& p_speed)
    {
      m_speed = p_speed;
    }
    void onUpdate(Timestep ts);
    void onEvent(Event& e);
    void onViewportResize(uint32_t width, uint32_t height);
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
    float m_zoom_speed = 3.0f, m_rotation_speed = 180.0f;
    Vec2 m_speed = { 5.0f, 5.0f };
    Vec4 m_initial = {-1.0f, 1.0f, -1.0f, 1.0f};
    bool m_can_control = true;
    bool m_control_zoom_with_mouse_scroll = true;
    OrthographicCamera m_camera;
  };
}

#endif //ORTHOGERAPHIC_CAMERA_CONTROLLER_H
