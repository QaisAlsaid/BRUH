#ifndef KR_CAMERA_H
#define KR_CAMERA_H

#include "Real-Engine/Core/Core.h"
#include "Real-Engine/Core/Math/math.h"


namespace Real
{
  class REAL_API Camera 
  {
  public:
    Camera() = default;
    explicit Camera(const Mat4& proj)
      : m_projection(proj) {}
    virtual ~Camera() = default;
    const Mat4& getProjection() const { return m_projection; }
  protected:
    Mat4 m_projection = Mat4(1.0f);
  };
}


#endif //KR_CAMERA_H
