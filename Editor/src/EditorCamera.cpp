#include "EditorCamera.h"
#include "Karen/Core/ButtonsAndKeyCodes.h"
#include "Karen/Core/Input.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/quaternion_common.hpp"


namespace Karen
{
  void EditorCamera::onUpdate(Timestep ts)
  {
    m_projection = glm::perspectiveRH(fov, aspect_ratio, near, far);
    
    auto mouse = Input::getMousePos();
    if(Input::isKeyPressed(Keyboard::Space))
    {
      Vec2 dmouse = m_last_mouse - mouse;
      m_last_mouse = mouse;
      position += Vec3(dmouse.x, -dmouse.y, 0.0f) * mouse_sensitivaty;
    }
    else 
    {
      m_last_mouse = mouse;
    }
  }

  Mat4 EditorCamera::getView() const
  {
    auto trans = Mat4(1.0f);
    auto pos = position;
    trans = glm::translate(trans, pos);
    trans = glm::rotate(trans, rotation.x, {1, 0, 0});
    trans = glm::rotate(trans, rotation.y, {0, 1, 0});
    trans = glm::rotate(trans, rotation.z, {0, 0, 1});
    return glm::inverse(trans);
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
    //fov = glm::radians(std::max(zoom, min_fov));
    //fov = glm::radians(std::min(zoom, max_fov));
    if(fov > max_fov) fov = max_fov;
    else if(fov < min_fov) fov = min_fov;
  }
}
