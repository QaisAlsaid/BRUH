#ifndef PHYSICS_WORLD2D_H
#define PHYSICS_WORLD2D_H


#include "Karen/Core/Core.h"
#include "Karen/Core/Timestep.h"
#include "Karen/Physics/Physics2D/RigidBody.h"
#include "Karen/Physics/Physics2D/Collisions.h"


namespace Karen
{
  class KAREN_API PhysicsWorld2D
  {
  public:
    PhysicsWorld2D();
    PhysicsWorld2D(size_t initial_size);
    //TEMP: TODO: make RigidBody2D movable and emplace it in the vector to avoid copying
    inline void add(const ARef<RigidBody2D>& body)
    {
      m_bodys.push_back(body);
    }
    inline void add(const ARef<RigidBody2D>& body, size_t idx)
    {
      const auto& iter = m_bodys.begin() + idx;
      m_bodys.insert(iter, body);
    }
    inline void remove()
    {
      m_bodys.pop_back();
    }
    //NOTE: it doesn't handel iterator invalidation
    inline void remove(size_t idx)
    {
      const auto& iter = m_bodys.begin() + idx;
      m_bodys.erase(iter);
    }
    inline const ARef<RigidBody2D>& get(size_t idx)
    {
      return m_bodys.at(idx);
    }
    inline size_t size() { return m_bodys.size(); }
    void onUpdate(Timestep ts);
  private:
    bool collide(const size_t& body1_idx, const size_t& body2_idx);
    bool collide(const ARef<RigidBody2D>& body1, const ARef<RigidBody2D>& body2);
  private:
    std::vector<ARef<RigidBody2D>> m_bodys;
  };
}

#endif //PHYSICS_WORLD2D_H
