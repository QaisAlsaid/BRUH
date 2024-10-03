#ifndef KR_ENTITY_H
#define KR_ENTITY_H

#include <entt/entt.hpp>
#include <flecs.h>
#include <utility>
#include "Real-Engine/Core/Core.h"
#include "Real-Engine/Core/UUID.h"
#include "Scene.h"
#include "flecs/addons/cpp/entity.hpp"


namespace Real
{
  class REAL_API Entity
  {
  public:
    Entity() = default;
    Entity(Entity& other, flecs::entity id, Scene* scene);
    Entity(flecs::entity id, Scene* scene);
    Entity(uint64_t id, Scene* scene);

    
    template<typename T>
    inline bool hasComponent() const
    {
      return m_id.has<T>();
    }

    inline bool isAlive() const 
    {
      return m_id.is_alive(); 
    }
    //template<typename... Ts>
    //inline bool hasAll() const
    //{
    //  return m_scene->m_registry.all_of<Ts...>(m_id);
    //}

    //template<typename... Ts>
    //inline bool hasAny() const
    //{
    //  return m_scene->m_registry.any_of<Ts...>(m_id);
    //}
  
    flecs::entity get()
    {
      return m_id;
    }

    template<typename T>
    inline T* getComponent() const
    {
      return m_id.get_mut<T>();
    }

    template<typename T>
    inline T* tryGetComponent() const
    {
      return getComponent<T>();
    }

    template<typename T, typename... TArgs>
    inline T* addComponent(TArgs&&... args) const
    {
      m_id.set(T{std::forward<TArgs>(args)...});
      return m_id.get_mut<T>();
    }

    template<typename T, typename... TArgs>
    inline T* insertComponent(TArgs&&... args) const
    {
      m_id.set(T{std::forward<TArgs>(args)...});
      return m_id.get_mut<T>();
    }

    template<typename T>
    inline void removeComponent() const
    {
      m_id.remove<T>();
    }

    template<typename T>
    inline void tryRemoveComponent() const
    {
      removeComponent<T>();
    }

    inline bool operator== (const Entity& other) const
    {
      return this->m_id == other.m_id;
    }

    inline bool operator!= (const Entity& other) const
    {
      return !(*this == other);
    }
    //inline operator uint32_t() const
    //{
    //  return (uint32_t)m_id;
    //}
    inline operator uint64_t() const
    {
      return (uint64_t)m_id;
    }
    //inline operator entt::entity() const
    inline operator flecs::entity() const
    {
      return m_id;
    }
    inline operator bool() const
    {
      return m_id != flecs::entity::null();
      //return m_id != entt::null;
    }
  private:
    inline void destroy() const 
    {
      m_id.destruct();
    }
  private:
    flecs::entity m_id;
    //entt::entity m_id{entt::null};
    Scene* m_scene;
    UUID m_uuid;
    
    friend class Scene;
  };
}

#endif //KR_ENTITY_H
