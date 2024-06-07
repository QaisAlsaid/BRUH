#ifndef KR_ENTITY_H
#define KR_ENTITY_H

#include <entt/entt.hpp>
#include "Karen/Core/Core.h"
#include "Karen/Core/UUID.h"
#include "Scene.h"


namespace Karen
{
  class KAREN_API Entity
  {
  public:
    Entity() = default;
    Entity(Entity& other, entt::entity id, Scene* scene);
    Entity(entt::entity id, Scene* scene);
    Entity(uint32_t id, Scene* scene);
    inline void inst(){}

    inline void destroy() const 
    {
      m_scene->removeEntity(*this);
    }

    template<typename T>
    inline bool hasComponent() const
    {
      return m_scene->m_registry.any_of<T>(m_id);
    }

    template<typename... Ts>
    inline bool hasAll() const
    {
      return m_scene->m_registry.all_of<Ts...>(m_id);
    }

    template<typename... Ts>
    inline bool hasAny() const
    {
      return m_scene->m_registry.any_of<Ts...>(m_id);
    }
  
    template<typename T>
    inline T& getComponent() const
    {
      return m_scene->m_registry.get<T>(m_id);
    }

    template<typename T>
    inline T* tryGetComponent() const
    {
      return m_scene->m_registry.try_get<T>(m_id);
    }

    template<typename T, typename... TArgs>
    inline T& addComponent(TArgs&&... args) const
    {
      return m_scene->m_registry.emplace<T>(m_id, std::forward<TArgs>(args)...);
    }
    template<typename T, typename... TArgs>
    inline T& insertComponent(TArgs&&... args) const
    {
      return m_scene->m_registry.emplace_or_replace<T>(m_id, std::forward<TArgs>(args)...);
    }

    template<typename T>
    inline void removeComponent() const
    {
      m_scene->m_registry.erase<T>(m_id);
    }

    template<typename T>
    inline void tryRemoveComponent() const
    {
      m_scene->m_registry.remove<T>(m_id);
    }

    inline bool operator== (const Entity& other) const
    {
      return this->m_id == other.m_id;
    }

    inline bool operator!= (const Entity& other) const
    {
      return !(*this == other);
    }
    inline operator uint32_t() const
    {
      return (uint32_t)m_id;
    }
    inline operator uint64_t() const
    {
      return (uint64_t)m_id;
    }
    inline operator entt::entity() const
    {
      return m_id;
    }
    inline operator bool() const
    {
      return m_id != entt::null;
    }
  private: 
    entt::entity m_id{entt::null};
    Scene* m_scene;
    UUID m_uuid;
  };
}

#endif //KR_ENTITY_H
