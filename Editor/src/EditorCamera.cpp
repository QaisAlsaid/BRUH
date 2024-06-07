#include "EditorCamera.h"
#include "Karen/Core/ButtonsAndKeyCodes.h"
#include "Karen/Core/Input.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/quaternion_common.hpp"


namespace Karen
{
  void EditorCamera::onUpdate(Timestep ts)
  {
    //vectors from Euler angles
    {
      front.x = cos(yaw) * cos(pitch);
      front.y = sin(pitch);
      front.z = sin(yaw) * cos(pitch);
      front = glm::normalize(front);
    }
    //find camera vectors
    {
      right = glm::normalize(glm::cross(front, world_up));
      up    = glm::normalize(glm::cross(right, front)); 
    }
    m_projection = glm::perspectiveRH(fov, aspect_ratio, near, far);
    
    //Mouse Movment
    {
      auto mouse = Input::getMousePos();
      Vec2 dmouse = m_last_mouse - mouse;
      Vec2 scaled_dmouse = dmouse * mouse_sensitivaty;
      if(Input::isKeyPressed(Keyboard::LeftAlt) || Input::isKeyPressed(Keyboard::RightAlt))
      {
        yaw += scaled_dmouse.x;
        pitch += scaled_dmouse.y;
        if(pitch >= pi) pitch = pi - 0.001;
        else if(pitch <= -pi) pitch = -(pi - 0.001);
      }
      m_last_mouse = mouse;
    }
  }

  Mat4 EditorCamera::getView() const
  {
    return glm::lookAtRH(position, position + front, world_up);
  }

  void EditorCamera::onResize(uint32_t w, uint32_t h)
  {
    if(!fixed_aspect_ratio)
      aspect_ratio = (float)w/(float)h;
  }

  void EditorCamera::onMouseScrolledEvent(MouseScrolledEvent& e)
  {
    auto offset = e.getYoffset();
    fov = glm::radians((glm::degrees(fov) - offset));
    if(fov > max_fov) fov = max_fov;
    else if(fov < min_fov) fov = min_fov;
  }
}
