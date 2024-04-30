#ifndef KR_SCENE_CAMERA_H
#define KR_SCENE_CAMERA_H

#include "Karen/Camera.h"


namespace Karen
{
  class SceneCamera : public Camera
  {
  public:
    SceneCamera() = default;
    //construct orthographic camera
    SceneCamera(Vec2 size);
    
    void setOrtho(Vec2 size);
    void setViewport(uint32_t width, uint32_t height);
  private:
    void reCalculate();
  private:
  private:
    float m_aspect_ratio   = 1280.0f/720.0f;
    float m_zoom_out_level = 1.0f;
    Vec2  m_size { 1.0f, 1.0f };
  };
}

#endif //KR_SCENE_CAMERA_H
