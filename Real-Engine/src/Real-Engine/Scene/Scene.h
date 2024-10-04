#ifndef KR_SCENE_H
#define KR_SCENE_H

#include "Real-Engine/Core/Core.h"
#include "Real-Engine/Core/Math/math.h"
#include "Real-Engine/Core/Timestep.h"
#include "Real-Engine/Core/UUID.h"
#include <entt/entt.hpp>



class b2World;

namespace Real
{
  class ScriptComponent;
  class Entity;

  class REAL_API Scene
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

    template<typename T>
    entt::view<T> view()
    {
      return m_registry.view<T>();
    }

    Entity getEntity(const std::string& name);
    Entity getEntity(UUID id);

    void onLoad();
    void onStart();
    void onUpdate(Timestep ts);
    void onEditorUpdate(Timestep ts);
    void setEditorCamera(const Mat4& view, const Mat4& projection) { m_editor_camera = { view, projection }; };
    void onViewportResize(uint32_t width, uint32_t height);
    void onEnd();
  private:
    std::string changeName(const std::string& name);
  private:
    std::string m_name;
    entt::registry m_registry;
    b2World* m_physics_world = nullptr;
    uint32_t m_viewport_width = 1280, m_viewport_height = 720;
    EditorCamera m_editor_camera;
                                //     Entity
    std::unordered_map<uint64_t, std::pair<uint32_t, Scene*>> m_fast_access; 
  private:
    friend class Entity;
    friend class SceneHierarchy;
    friend class SceneSerializer;
  };
}


#endif //KR_SCENE_H
