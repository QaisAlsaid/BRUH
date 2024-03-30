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
//TODO: Move render code to Test
namespace Karen
{
  App* App::stat_instance = nullptr;
  App::App()
  {
    RendererAPI::setAPI(RendererAPI::API::OpenGl);
    Vec2 x(10, 10);
    KAREN_TRACE("{0}, {1}", x.x, x.y);
    KAREN_CORE_ASSERT(!stat_instance, "App Exist");
    stat_instance = this;
    m_window = std::unique_ptr<Window>(Window::create());
    m_window->setEventCallbackFunction(BIND_EVENT_FUNCTION(App::onEvent));
    m_gui_layer = new GuiLayer;
    pushOverlay(m_gui_layer); 


    float data[] =
    {
      -0.5f, 0.0f, 0.0f,
      0.5f, 0.0f, 0.0f,
      0.5f, 0.5f, 0.0f
    };

    uint32_t ind[3]
    {
      0,1,2
    };

    BufferLayout bl
    {
      {"pos", ShaderDataType::Float3}
    };

    m_v_buff.reset(VertexBuffer::create(sizeof(data), data, 9));
    m_shader.reset(Shader::create());
    m_i_buff.reset(IndexBuffer::create(3, ind, 3));
    m_v_arr.reset(VertexArray::create());
    
    m_shader->loadFromFile("../res/shaders/vs.glsl", "../res/shaders/fs.glsl");
    m_v_buff->setLayout(bl);
    m_v_arr->addVertexBuffer(m_v_buff);
    m_v_arr->setIndexBuffer(m_i_buff);
    KAREN_CORE_INFO("Layout : {0}",m_v_arr->getVertexBuffers().at(0)->getLayout().toString());
  }
  App::~App()
  {
  }
  void App::onEvent(Event& event)
  {
    EventDispatcher dp(event);
    dp.dispatch<WindowClosedEvent>(BIND_EVENT_FUNCTION(App::onCloseCall));
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
    if(!event.isHandled())
      KAREN_CORE_TRACE("{0}", event);
  }
  void App::run()
  {
    KAREN_CORE_INFO("Welcome To Karen Engine!");
    while(m_running)
    {
      glClearColor(0.2f, 0.7f, 0.9f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      for(Layer* layer : m_layers)
      {
        layer->onUpdate();
      }
      m_v_arr->bind();
      m_shader->bind();
      m_i_buff->bind();
      m_v_buff->bind();
      glDrawElements(GL_TRIANGLES, m_i_buff->getCount(), GL_UNSIGNED_INT, nullptr);
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
