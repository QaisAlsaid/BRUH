#ifndef KR_ENTITY_H
#define KR_ENTITY_H

#include <entt/entt.hpp>
#include "Scene.h"


namespace Karen
{
  class Entity
  {
  public:
    Entity() = default;
    Entity(entt::entity id, Scene* scene);
    /*bool hasComponent()
      {
        m_scene->m_registry.orphan<T>(m_id);
      }*/
  
    template<typename T>
    T& getComponent()
    {
      return m_scene->m_registry.get<T>(m_id);
    }

    template<typename T, typename... TArgs>
    T& addComponent(TArgs&&... args)
    {
      return m_scene->m_registry.emplace<T>(m_id, std::forward<TArgs>(args)...);
    }

    template<typename T>
    void removeComponent()
    {
      m_scene->m_registry.remove<T>(m_id);
    }

  private: 
    entt::entity m_id{0};
    Scene* m_scene;
  };
}

#endif //KR_ENTITY_H
