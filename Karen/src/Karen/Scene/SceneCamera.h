#ifndef KR_SCENE_CAMERA_H
#define KR_SCENE_CAMERA_H

#include "Karen/Camera.h"


namespace Karen
{
  class SceneCamera : public Camera
  {
  public:
    struct OrthographicData
    {
      float size      =  1.0f;
      float near_clip = -1.0f;
      float far_clip  =  1.0f;
    };
    struct PerspectiveData
    {
      float fov = 45.0f;
      float near_clip = 0.0f;
      float far_clip  = 100.0f; 
    };
    enum class ProjectionType
    {
      Orthographic = 0, Perspective = 1
    };
  public:
    SceneCamera() = default;
    //construct orthographic camera
    SceneCamera(const OrthographicData& data);
    SceneCamera(const PerspectiveData& data);

    void setOrthographicData(const OrthographicData& data);
    void setPerspectiveData(const PerspectiveData& data);
    void setOrthographicData(float size, float near_clip = -1.0f, float far_clip = 1.0f);
    void setPerspectiveData(float fov, float near_clip = 0.0f, float far_clip = 1000.0f);
    
    void setViewport(uint32_t width, uint32_t height);
    inline void setType(ProjectionType t) 
    { 
      if(t != m_type)
      {
        m_type = t; 
        reCalculate();
      }
    }
    inline       ProjectionType    getType() const        { return m_type; }
    inline const OrthographicData& getOrthographicData()  { return m_ortho; }
    inline const PerspectiveData&  getPerspectiveData()   { return m_persp; } 
  private:
    void reCalculate();
  private:
  private:
    float m_aspect_ratio   = 1280.0f/720.0f;
    float m_zoom_out_level = 1.0f;
    OrthographicData m_ortho;
    PerspectiveData  m_persp;
    ProjectionType   m_type = ProjectionType::Orthographic;
  };
}

#endif //KR_SCENE_CAMERA_H
