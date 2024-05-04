#ifndef KR_INSPECTOR_H
#define KR_INSPECTOR_H

#include "Karen/Karen.h"


namespace Karen
{
  class Inspector
  {
  public:
    Inspector() = default;
    
    //set to null to unselect;
    void setCurrentSelected(Entity entity) { m_current = entity; }
    void onGuiUpdate();
  private:
    Entity m_current;
  };
}


#endif //KR_INSPECTOR_H
