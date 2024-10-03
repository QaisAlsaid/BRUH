#ifndef KR_SCRIPT_H
#define KR_SCRIPT_H

#include "Real-Engine/Core/Timestep.h"
#include "Real-Engine/Scene/Entity.h"


namespace Real
{
  class Script
  {
  public:
    std::function<void(void)> onCreate = []()  { REAL_CORE_ERROR("Uninitalized onCreate");  };
    std::function<void(void)> onUpdate = []()  { REAL_CORE_ERROR("Uninitalized onUpdate");  };
    std::function<void(void)> onDestroy = []() { REAL_CORE_ERROR("Uninitalized onDestroy"); };
    Entity entity;
    Timestep getTimestep() { return timestep; }
  private:
    Timestep timestep;
    friend class Scene;
  };
}

#endif //KR_SCRIPT_H
