#ifndef KR_SCENE_H
#define KR_SCENE_H

#include "Karen/Core/Timestep.h"
#include <entt/entt.hpp>



class b2World;

namespace Karen
{ 
  class Entity;

  class Scene
  {
  public:
    Scene(const std::string& name = "Scene");
    Entity addEntity(const std::string& tag = "");
    void removeEntity(const Entity& entity);
    void clear();

    void onStart();
    void onUpdate(Timestep ts);
    void onEditorUpdate(Timestep ts);
    void onViewportResize(uint32_t width, uint32_t height);
    void onEnd();
  private:
    std::string m_name;
    entt::registry m_registry;
    b2World* m_physics_world = nullptr;
    uint32_t m_viewport_width = 1280, m_viewport_height = 720;
    friend class Entity;
    friend class SceneHierarchy;
    friend class SceneSerializer;
  };
}


#endif //KR_SCENE_H
