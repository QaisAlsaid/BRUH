#include "pch.h"
#include "Karen/Core/Timestep.h"
#include "Karen/Core/Events/AppEvents.h"
#include "Karen/Core/App.h"
#include "Karen/Core/Log.h"
#include "Karen/Render/API/RendererAPI.h"

#ifdef KAREN_EMSCRIPTEN
#include <emscripten.h>
static void callMain(void* fp)
{
  std::function<void()>* func = (std::function<void()>*)fp;
  (*func)();
}
#endif //KAREN_EMSCRIPTEN

//FIXME:remove it
#include <GLFW/glfw3.h>

namespace Karen
{
  App* App::stat_instance = nullptr;
  App::App()
  {
    KAREN_CORE_INFO("App Created");
    RendererAPI::setAPI(RendererAPI::API::OpenGl);
    KAREN_CORE_ASSERT_MSG(!stat_instance, "App Exist");
    stat_instance = this;
    m_window = std::unique_ptr<Window>(Window::create());
    m_window->setEventCallbackFunction(BIND_EVENT_FUNCTION(App::onEvent));
    m_gui_layer = new GuiLayer("Base GuiLayer");
    m_gui_layer->activate();
    pushOverlay(m_gui_layer);
   }

  App::~App()
  {
  }

  void App::onEvent(Event& event)
  {
    EventDispatcher dp(event);
    dp.dispatch<WindowClosedEvent>(BIND_EVENT_FUNCTION(App::onCloseCall));
    dp.dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(App::onResizeCall));
    
    for(auto it = m_layers.end(); it != m_layers.begin();)
    {
      --it;
      if((*it)->isActive())
      {
        if(event.isHandled())
          break;
        (*it)->onEvent(event);
      }
    }
  }

  void App::run()
  {
#ifndef KAREN_EMSCRIPTEN
    while(m_running)
    {
#else 
      std::function<void()> MainLoop = [&] () {
#endif
      float time = glfwGetTime();
      float delta_time = time - m_last_time;
      m_last_time = time;
      Timestep ts(delta_time);
      for(auto it = m_layers.end(); it != m_layers.begin();)
      {
        --it;
        if((*it)->isActive())
        {
          (*it)->onUpdate(ts);
          KAREN_CORE_INFO("called onUpdate for Layer: {0}, isActive: {1}",(*it)->getName(), (*it)->isActive());
        }
      }

      m_gui_layer->begin();
      for(auto it = m_layers.end(); it != m_layers.begin();)
      {
        --it;
        if((*it)->isActive() && (*it)->isGuiActive())
        {
          (*it)->onGuiUpdate();
          KAREN_CORE_INFO("called onGuiUpdate for Layer: {0}, isActive: {1}, isGuiActive: {2}",(*it)->getName(), (*it)->isActive(), (*it)->isGuiActive());
        }
      }
      m_gui_layer->end();


      m_window->onUpdate();
#ifndef KAREN_EMSCRIPTEN
    }
#else 
    };
    emscripten_set_main_loop_arg(callMain, &MainLoop, 0, 1);
#endif //KAREN_EMSCRIPTEN
  }
  bool App::onCloseCall(WindowClosedEvent& event)
  {
    m_running = false;
    return true;
  }

  bool App::onResizeCall(WindowResizeEvent& event)
  {
    return false;
  }
  
  void App::pushLayer(Layer* layer)
  {
    m_layers.pushLayer(layer);
    layer->onAttach();
  }

  void App::pushOverlay(Layer* layer)
  {
    m_layers.pushOverlay(layer);
    layer->onAttach();
  }

  void App::popLayer(Layer* layer)
  {
    m_layers.popLayer(layer);
    layer->onDetach();
  }

  void App::popOverlay(Layer* layer)
  {
    m_layers.popOverlay(layer);
    layer->onDetach();
  }
}
