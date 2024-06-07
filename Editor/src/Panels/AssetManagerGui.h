#ifndef KR_ASSET_MANAGER_GUI_H
#define KR_ASSET_MANAGER_GUI_H

#include <Karen/Karen.h>

namespace Karen
{
  class AssetManagerPanel
  {
  public:
    AssetManagerPanel(AssetManager* context);
    
    void setContext(AssetManager* context) { m_context = context; }
    void onImGuiUpdate();
  private:
    AssetManager* m_context;
    bool m_is_active = true;
  };

  class AssetManagerModal
  {
  public:
    enum class TypeEnum {Texture2D, Scene, None};
    struct LoadedType
    {
      std::string path;
      TypeEnum type;
    };
  public:
    AssetManagerModal(AssetManager* context);

    void setContext(AssetManager* context) { m_manager_context = context; }
    void onImGuiUpdate();
    inline void show() { m_is_active = true; }
    void setToLoadContext(LoadedType t) { m_to_load_context = t; };
  private:
    AssetManager* m_manager_context;
    LoadedType      m_to_load_context;
    bool m_is_active = false;
  };
}


#endif //KR_ASSET_MANAGER_PANEL_H
