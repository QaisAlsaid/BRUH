#ifndef KR_MENU_BAR_H
#define KR_MENU_BAR_H

#include <Karen/Karen.h>
#include "HelperWindows.h"


namespace Karen
{
  class MenuBar
  {
  public:
    MenuBar() = default;
    MenuBar(StatsWindow* stats_window);
  
    void onImGuiUpdate();
  public:
    StatsWindow* stats_window;//TODO: weakRef
  };
}


#endif //KR_MENU_BAR_H
