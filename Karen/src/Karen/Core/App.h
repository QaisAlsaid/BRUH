#ifndef APP_H
#define APP_H

#include "Karen/Core/Core.h"
#include "Karen/Core/Window.h"
#include "Karen/Core/Layers/Layer.h"
#include "Karen/Core/Layers/LayerStack.h"
#include "Karen/Core/Layers/GuiLayer.h"
#include "Karen/Core/Events/Events.h"
#include "Karen/Core/AssetManager.h"


namespace Karen
{
  class KAREN_API App
  {
  public:
    App();
    virtual ~App();
    
    void run();
    void onEvent(Event&);
    
    inline Window& getWindow() const
    {
      return *m_window;
    }

    inline AssetManager& assetManager()
    {
      return m_asset_manager;
    }

    inline const AssetManager& assetManager() const
    {
      return m_asset_manager;
    }
    
    inline static App* get()
    {
      KAREN_CORE_ASSERT_MSG(s_instance, "No App Created");
      return s_instance;
    }
  private:
    float m_last_time = 0.0f;
    AssetManager m_asset_manager;
    std::unique_ptr<Window> m_window;
    GuiLayer* m_gui_layer = nullptr;
    bool m_running = true;
    LayerStack m_layers;
    static App* s_instance;
  protected:
    void pushLayer(Layer* layer);
    void pushOverlay(Layer* layer);
    void popLayer(Layer* layer);
    void popOverlay(Layer* layer);
    bool onResizeCall(WindowResizeEvent& event);
    bool onCloseCall(WindowClosedEvent&);
  };

  KAREN_API App* createApp();
}
#endif //APP_H
