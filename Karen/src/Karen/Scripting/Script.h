#ifndef KR_SCRIPT_H
#define KR_SCRIPT_H

#include "Karen/Core/App.h"
#include "Karen/Core/ExportType.h"
#include "Karen/Core/Timestep.h"
#include "Karen/Scene/Entity.h"


namespace Karen
{
  class Script
  {
  public:
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
