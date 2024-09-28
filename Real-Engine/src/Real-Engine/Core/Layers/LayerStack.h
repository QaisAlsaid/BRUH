#ifndef LAYER_STACK_H
#define LAYER_STACK_H

#include "Real-Engine/Core/Layers/Layer.h"
#include "Real-Engine/Core/Core.h"

namespace Real
{
  typedef std::vector<Layer*> LayerVec;
  typedef LayerVec::iterator LayerStackIterator;

  class REAL_API LayerStack
  {
  public:
    LayerStack();
    ~LayerStack();

    void pushLayer(Layer* layer);
    void pushOverlay(Layer* layer);
    void popLayer(Layer* layer);
    void popOverlay(Layer* layer);
    
    LayerStackIterator begin() {return m_layers.begin();}
    LayerStackIterator end()   {return m_layers.end();}
    //FIXME: const iterator:
    //const LayerStackIterator begin() const {return m_layers.begin();}
    //const LayerStackIterator end()   const {return m_layers.end();}
  private:
    LayerVec     m_layers;
    unsigned int m_index;
  };
}

#endif //LAYER_STACK_H
