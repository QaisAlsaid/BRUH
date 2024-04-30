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

    void onStart();
    void onUpdate(Timestep ts);
    void onViewportResize(uint32_t width, uint32_t height);
    void onEnd();
  private:
    entt::registry m_registry;
    uint32_t m_viewport_width = 1280, m_viewport_height = 720;
    friend class Entity;
  };
}


#endif //KR_SCENE_H
