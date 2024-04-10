#ifndef ORTHOGRAPHIC_CAMERA_H
#define ORTHOGRAPHIC_CAMERA_H

#include "Karen/Core/Core.h"
#include "Karen/Core/Math/math.h"


namespace Karen
{
  class KAREN_API OrthographicCamera
  {
  public:
    OrthographicCamera(float p_left, float p_right, float p_bottom, float p_top, Vec3 p_pos = Vec3(0.0f));
    OrthographicCamera();

    void setRect(float p_left, float p_right, float p_bottom, float p_top);
    inline Vec4 getRect() 
    {
      return Vec4(m_left, m_right, m_bottom, m_top);
    }
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
      if(p_zoom > m_min_zoom) m_zoom = m_min_zoom;
      else if(p_zoom < m_min_zoom) m_zoom = m_min_zoom;
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
    inline void setZoomLimits(float min_zoom, float max_zoom)
    {
      m_min_zoom = min_zoom;
      m_max_zoom = max_zoom;
    }
    inline float getMinZoom()
    {
      return m_min_zoom;
    }
    inline float getMaxZoom()
    {
      return m_max_zoom;
    }
  private:
    float m_left = -1.0f, m_right = 1.0f, m_bottom = -1.0f, m_top = 1.0f;
    float m_zoom = 1.0f, m_min_zoom = 0.25f, m_max_zoom = 10.0f;
    float m_rotation = 0.0f;
    Vec3 m_pos   = { 0.0f, 0.0f, 0.0f };
    Mat4 m_proj;
    Mat4 m_view;
    Mat4 m_pv;
  };
}

#endif //ORTHOGRAPHIC_CAMERA_H
