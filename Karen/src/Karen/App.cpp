#include "Karen/CommanUtils/FileLoader.h"
#include "Karen/Core/Timestep.h"
#include "Karen/Events/AppEvents.h"
#include "Render/API/RenderCommands.h"
#include "Render/API/VertexArray.h"
#include "pch.h"
#include "Karen/App.h"
#include "Karen/ButtonsAndKeyCodes.h"
#include "Karen/Log.h"
#include "Karen/Input.h"
#include "Karen/Math/math.h"
#include "Render/API/RendererAPI.h"
#include "Render/API/VertexBuffer.h"
#include "Render/API/IndexBuffer.h"
#include "Render/API/BufferLayout.h"

//FIXME:remove it
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Karen
{
  App* App::stat_instance = nullptr;
  App::App()
  {
    RendererAPI::setAPI(RendererAPI::API::OpenGl);
    KAREN_CORE_ASSERT(!stat_instance, "App Exist");
    stat_instance = this;
    m_window = std::unique_ptr<Window>(Window::create());
    m_window->setEventCallbackFunction(BIND_EVENT_FUNCTION(App::onEvent));
    m_gui_layer = new GuiLayer;
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
    
    for(auto it = m_layers.end(); it != m_layers.end();)
    {
      (*--it)->onEvent(event);
      if(event.isHandled())
        break;
    }
    
    for(Layer* l : m_layers)
    {
      l->onEvent(event);
    }  
  }

  void App::run()
  {
    while(m_running)
    {
      float time = glfwGetTime();
      float delta_time = time - m_last_time;
      m_last_time = time;
      Timestep ts(delta_time);
      for(Layer* layer : m_layers)
      {
        layer->onUpdate(ts);
      }

      m_gui_layer->begin();
      for(auto& layer : m_layers)
        layer->onGuiUpdate();
      m_gui_layer->end();
      m_window->onUpdate();
    }
  }
  bool App::onCloseCall(WindowClosedEvent& event)
  {
    m_running = false;
    return true;
  }

  bool App::onResizeCall(WindowResizeEvent& event)
  {
    //glViewport(0,0,event.getWidth(),event.getHeight());
    return true;  
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
