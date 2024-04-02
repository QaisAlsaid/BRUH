#ifndef ORTHOGRAPHIC_CAMERA_H
#define ORTHOGRAPHIC_CAMERA_H

#include "Karen/Core.h"
#include "Karen/Math/math.h"
#include "glm/ext/matrix_clip_space.hpp"

#define ZOOM 45.0f
#define MIN_ZOOM 1.0f
#define MAX_ZOOM 90.0f

namespace Karen
{
  class KAREN_API OrthographicCamera
  {
  public:
    OrthographicCamera(float p_left, float p_right, float p_bottom, float p_top, Vec3 p_pos = Vec3(0.0f));
    OrthographicCamera();

    void setRect(float p_left, float p_right, float p_bottom, float p_top);
    void onUpdate(float delta);
    inline const Mat4& getView() const
    {
      return m_view;
    }

    inline const Mat4& getProj() const
    {
      return m_proj;
    }

    inline const Mat4& getProjView() const
    {
      return m_pv;
    }

    inline void setPosition(Vec3 p_pos)
    {
      m_pos = p_pos;
    }

    inline const Vec3& getPosition() const
    {
      return m_pos;
    }

    inline void setZoom(float p_zoom)
    {
      if(p_zoom > MAX_ZOOM) m_zoom = MAX_ZOOM;
      else if(p_zoom < MIN_ZOOM) m_zoom = MIN_ZOOM;
      else m_zoom = p_zoom;
    }

    inline float getZoom() const
    {
      return m_zoom;
    }

    inline void setRotation(float p_angle)
    {
      m_rotation = p_angle;
    }

    inline float getRotation() const
    {
      return m_rotation;
    }
  private:
    float m_left = -1.0f, m_right = 1.0f, m_bottom = -1.0f, m_top = 1.0f;
    float m_zoom = ZOOM;
    float m_rotation = 0.0f;
    Vec3 m_pos   = { 0.0f, 0.0f, 0.0f };
    Mat4 m_proj;
    Mat4 m_view;
    Mat4 m_pv;
  };
}

#endif //ORTHOGRAPHIC_CAMERA_H
