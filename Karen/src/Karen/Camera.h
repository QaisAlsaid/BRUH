#ifndef KR_CAMERA_H
#define KR_CAMERA_H

#include "Karen/Core/Math/math.h"


namespace Karen
{
  class Camera 
  {
  public:
    Camera() = default;
    Camera(const Mat4& proj)
      : m_projection(proj) {}
    
    const Mat4& getProjection() const { return m_projection; }
  private:
    Mat4 m_projection;
  };
}


#endif //KR_CAMERA_H
