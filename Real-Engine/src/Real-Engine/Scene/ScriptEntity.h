#ifndef KR_SCRIPT_ENTITY
#define KR_SCRIPT_ENTITY

#include "Real-Engine/Core/Core.h"
#include "Entity.h"
#include "Real-Engine/Core/Timestep.h"


//TODO: FIXME: refactor plssss
namespace Real
{
  class REAL_API ScriptEntity
  {
  public:
    virtual ~ScriptEntity() = default;
    virtual Entity getEntity() { return m_entity; }
    virtual void onCreate()            {}
    virtual void onUpdate(Timestep ts) {}
    virtual void onDestroy()           {}
    
  protected:
    Entity m_entity;

    friend class Scene;
  };
}


#endif //KR_SCRIPT_ENTITY
