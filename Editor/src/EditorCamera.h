#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H

#include "Karen/Camera.h"
#include "Karen/Core/Events/MouseEvents.h"
#include "Karen/Core/Math/math.h"
#include "Karen/Core/Timestep.h"


namespace Karen
{
  constexpr auto pi = glm::pi<float>();

  class EditorCamera : public Camera
  {
  public:
    void onUpdate(Timestep ts);
    Mat4 getView() const;
    void onResize(uint32_t width, uint32_t height);
    void onMouseScrolledEvent(MouseScrolledEvent& e);
  public:
    Vec3 position = { 0.0f, 0.0f, 1.0f };
    Vec3 rotation = { 0.0f, 0.0f, 0.0f };
    float fov = pi/4.0f;
    float aspect_ratio = 16.0f/9.0f;
    float near = 0.1f, far = 1000.0f;
    float mouse_sensitivaty = 0.01f;
    float min_fov = glm::radians(0.1f), max_fov = pi / 2.0f;
    bool fixed_aspect_ratio = false;
  private:
    Vec2 m_last_mouse = { 0.0f, 0.0f };
  };
}

#endif //EDITOR_CAMERA_H
