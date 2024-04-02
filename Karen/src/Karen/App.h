#ifndef APP_H
#define APP_H

///////TODO:remove it
#include "Render/API/IndexBuffer.h"
#include "Render/API/VertexArray.h"
#include "Render/API/VertexBuffer.h"
#include "Render/API/Shader.h"
#include "Render/OrthographicCamera.h"
//////

#include "Karen/Core.h"
#include "Karen/Window.h"
#include "Karen/Layers/Layer.h"
#include "Karen/Layers/LayerStack.h"
#include "Karen/Layers/GuiLayer.h"
#include "Karen/Events/Events.h"


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
    inline static App* get()
    {
      KAREN_CORE_ASSERT(stat_instance, "No App Created");
      return stat_instance;
    }
  private:
    ARef<Shader> m_p;
    std::unique_ptr<Window> m_window;
    GuiLayer* m_gui_layer = nullptr;
    bool m_running = true;
    LayerStack m_layers;
    static App* stat_instance;
    OrthographicCamera m_ortho = OrthographicCamera(-1.0f, 1.0f, -1.0f, 1.0f);
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
