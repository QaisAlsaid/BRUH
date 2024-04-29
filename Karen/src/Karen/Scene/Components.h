#ifndef KR_COMPONENTS_H
#define KR_COMPONENTS_H

#include "Karen/Camera.h"
#include "Karen/Core/Math/math.h"


namespace Karen
{
  struct TagComponent
  {
    std::string name;
    TagComponent() = default;
    TagComponent(const std::string& s = "") : name(s) {}
  };

  struct TransformComponent
  {
    Vec3 position  = {0.0f, 0.0f, 0.0f};
    Vec2 scale     = {1.0f, 1.0f};
    float rotation = 0.0f;
    TransformComponent() = default;
    TransformComponent(const Vec3& pos, const Vec2& scale, float rotation)
      : position(pos), scale(scale), rotation(rotation) {}
    Mat4 getTransformationMatrix()
    {
      Mat4 trans(1.0f);
      trans = glm::translate(trans, position);
      trans = glm::rotate(trans, rotation, {0, 0, 1});
      trans = glm::scale(trans, Vec3(scale, 1.0f));
      return trans;
    }
  };

  struct SpriteComponent
  {
    Vec4 color;
    SpriteComponent() = default;
    SpriteComponent(const Vec4& color) : color(color) {}
  };

  struct CameraComponent
  {
    Camera camera;
    bool is_primary = false;
    CameraComponent() = default;
    CameraComponent(const Camera& camera)
      : camera(camera) {}
    CameraComponent(const Mat4& projection)
      : camera(projection) {}
  };
}


#endif //KR_COMPONENTS_H
