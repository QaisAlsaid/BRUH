#ifndef KR_SCRIPT_ENTITY
#define KR_SCRIPT_ENTITY

#include "Karen/Core/Core.h"
#include "Entity.h"
#include "Karen/Core/Timestep.h"


//TODO: FIXME: refactor plssss
namespace Karen
{
  class KAREN_API ScriptEntity
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
