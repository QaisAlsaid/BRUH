#ifndef KR_SCRIPT_H
#define KR_SCRIPT_H

#include "Karen/Core/Timestep.h"
#include "Karen/Scene/Entity.h"
#include "Lua.h"
#include <sol/forward.hpp>

struct DynamicObject;

namespace Karen
{
  class Script : public DynamicObject
  {
  public:
    //void _onCreate();
    //void _onUpdate(Timestep ts);
    //void _onDestroy();
    //sol::function onCreate;
    //sol::function onUpdate;
    //sol::function onDestroy;
    std::function<void(void)> onCreate;
    std::function<void(void)> onUpdate;
    std::function<void(void)> onDestroy;
    Entity entity;
    Timestep getTimestep() { return timestep; }
  private:
    Timestep timestep;
    friend class Scene;
  };
}

#endif //KR_SCRIPT_H
