#include "pch.h"
#include "Real-Engine/Core/Layers/LayerStack.h"

namespace Real
{
  LayerStack::LayerStack()
  {
    m_index = 0;
  }

  LayerStack::~LayerStack()
  {
    for(Layer* layer : m_layers)
    {
      delete layer;
    }
  }

  void LayerStack::pushLayer(Layer* layer)
  {
    m_layers.emplace(m_layers.begin() + m_index, layer);
    m_index++;
  }

  void LayerStack::pushOverlay(Layer* layer)
  {
    m_layers.emplace_back(layer);
  }

  void LayerStack::popLayer(Layer* layer)
  {
    auto it = std::find(m_layers.begin(), m_layers.end(), layer);
    if(it != m_layers.end())
    {
      m_layers.erase(it);
      m_index--;
      delete layer;
    }
  }

  void LayerStack::popOverlay(Layer* layer)
  {
    auto it = std::find(m_layers.begin(), m_layers.end(), layer);
    if(it != m_layers.end())
    {
      m_layers.erase(it);
      delete layer; 
    }
  }
}
