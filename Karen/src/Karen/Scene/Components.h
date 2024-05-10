#ifndef KR_COMPONENTS_H
#define KR_COMPONENTS_H

#include "Karen/Render/API/Texture.h"
#include "SceneCamera.h"
#include "ScriptEntity.h"
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
    Vec3 scale     = {1.0f, 1.0f, 1.0f};
    Vec3 rotation  = {0.0f, 0.0f, 0.0f};
    TransformComponent() = default;
    TransformComponent(const Vec3& pos, const Vec3& scale, const Vec3& rotation)
      : position(pos), scale(scale), rotation(rotation) {}
    Mat4 getTransformationMatrix()
    {
      Mat4 trans(1.0f);
      trans = glm::translate(trans, position);
      trans = glm::rotate(trans, rotation.x, {1, 0, 0});
      trans = glm::rotate(trans, rotation.y, {0, 1, 0});
      trans = glm::rotate(trans, rotation.z, {0, 0, 1});
      trans = glm::scale(trans, scale);
      return trans;
    }
  };

  struct SpriteComponent
  {
    std::string texture_handel;
    Vec4 color;
    SpriteComponent() = default;
    SpriteComponent(const Vec4& color, const std::string& tux) : texture_handel(tux), color(color) {}
  };

  struct CameraComponent
  {
    SceneCamera camera;
    bool is_primary = false;
    bool is_fixed_aspect_ratio = false;
    CameraComponent() = default;
  };

  struct NativeScriptComponent
  {
    ScriptEntity* instance = nullptr;
     
    template<typename T>
    void bind()
    {
      instantiateScript = [](){return static_cast<ScriptEntity*>(new T());};
      DestroyScript = [](NativeScriptComponent* native_script)
      {
        delete native_script->instance;
        native_script->instance = nullptr;
      };
    }
    private:
      ScriptEntity* (*instantiateScript)();
      void (*DestroyScript)(NativeScriptComponent*);
    friend class Scene;
  };
}


#endif //KR_COMPONENTS_H
