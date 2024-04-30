#ifndef KR_SCRIPT_ENTITY
#define KR_SCRIPT_ENTITY

#include "Entity.h"
#include "Karen/Core/Timestep.h"

namespace Karen
{
  class ScriptEntity
  {
  public:
    virtual ~ScriptEntity() = default;
  protected:  
    virtual void onCreate()            {}
    virtual void onUpdate(Timestep ts) {}
    virtual void onDestroy()           {}
    
    Entity m_entity;

    friend class Scene;
  };
}


#endif //KR_SCRIPT_ENTITY
