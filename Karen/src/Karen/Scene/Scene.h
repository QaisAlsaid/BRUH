#ifndef KR_SCENE_H
#define KR_SCENE_H

#include "Karen/Core/Core.h"
#include "Karen/Core/Math/math.h"
#include "Karen/Core/Timestep.h"
#include "Karen/Core/UUID.h"
#include <entt/entt.hpp>



class b2World;

namespace Karen
{ 
  class Entity;

  class KAREN_API Scene
  {
    struct EditorCamera { Mat4 view, projection; };
  public:
    Scene(const std::string& name = "Scene");
    Scene(const Scene& other);
    
    static ARef<Scene> copy(const ARef<Scene>&);

    Entity addEntity(const std::string& tag = "");
    Entity addEntity(UUID uuid, const std::string& tag = "");
    Entity copyEntity(Entity& e);
    void removeEntity(const Entity& entity);
    void clear();
    
    template<typename T>//TODO: ...
    void forEach(std::function<void(entt::entity, T&)> func)
    {
      m_registry.view<T>().each(func);
    }

    void onStart();
    void onUpdate(Timestep ts);
    void onEditorUpdate(Timestep ts);
    void setEditorCamera(const Mat4& view, const Mat4& projection) { m_editor_camera = { view, projection }; };
    void onViewportResize(uint32_t width, uint32_t height);
    void onEnd();
  private:
    std::string m_name;
    entt::registry m_registry;
    b2World* m_physics_world = nullptr;
    uint32_t m_viewport_width = 1280, m_viewport_height = 720;
    EditorCamera m_editor_camera;
  private:
    friend class Entity;
    friend class SceneHierarchy;
    friend class SceneSerializer;
  };
}


#endif //KR_SCENE_H
