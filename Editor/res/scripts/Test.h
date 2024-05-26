#ifndef TEST_H
#define TEST_H

#ifndef KAREN_BUILD_SHARED
#define KAREN_BUILD_SHARED
#define WE_DID_IT
#endif
#include "Karen/Karen.h"


class KAREN_API TestScript : public Karen::ScriptEntity
{
public:
  TestScript()  {}
  ~TestScript() {}
  virtual Karen::Entity getEntity() override;
  virtual void onCreate() override; 
  virtual void onUpdate(Karen::Timestep) override;
  virtual void onDestroy() override;
};

#ifdef WE_DID_IT
#undef KAREN_BUILD_SHARED
#endif
#endif //TEST_H
