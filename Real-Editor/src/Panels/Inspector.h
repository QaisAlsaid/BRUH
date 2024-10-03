#ifndef KR_INSPECTOR_H
#define KR_INSPECTOR_H

#include "EditorEvents/EditorEvents.h"
#include "EditorEvents/MousePickEvents.h"
#include "EditorEvents/SceneHierarchyPanelEvents.h"
#include "Real-Engine/Real-Engine.h"


namespace Real
{
  class Inspector
  {
  public:
    Inspector();
    
    //set to null to unselect;
    void setCurrentSelected(Entity entity) { m_current = entity; }
    Entity getCurrentSelected()            { return m_current;   }
    void onGuiUpdate();
    
    void onEditorEvent(EditorEvent& e);
    bool onMousePickedChangedCall(MousePickedChangedEvent&);
    bool onSCHPSelectionChangedCall(SCHPSelectionChangedEvent&);
  private:
    void drawExportedVars(UUID uuid);
    Entity m_current;
    uint32_t m_sub_id;
  };
}


#endif //KR_INSPECTOR_H
