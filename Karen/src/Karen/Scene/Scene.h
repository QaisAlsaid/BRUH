#ifndef KR_SCENE_H
#define KR_SCENE_H

#include "Karen/Core/Timestep.h"
#include <entt/entt.hpp>

namespace Karen
{
  class Entity;

  class Scene
  {
  public:
    Entity addEntity(const std::string& tag = "");
    //void removeEntity();
    //void clear();

    void onUpdate(Timestep ts);
  private:
    entt::registry m_registry;

    friend class Entity;
  };
}


#endif //KR_SCENE_H
