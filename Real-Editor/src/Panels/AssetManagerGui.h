#ifndef KR_ASSET_MANAGER_GUI_H
#define KR_ASSET_MANAGER_GUI_H

#include <Karen/Karen.h>

namespace Karen
{
  class AssetManagerModal
  {
  public:
    void show() { m_is_shown = true; }
    void onImGuiUpdate();
    void setContext(UUID ctx) { m_context = ctx; }
  private:
    UUID m_context = UUID::invalid;
    bool m_is_shown = false;
  };
}


#endif //KR_ASSET_MANAGER_PANEL_H
