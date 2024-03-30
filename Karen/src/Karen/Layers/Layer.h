#ifndef LAYER_H
#define LAYER_H


#include "Karen/Core.h"
#include "Karen/Events/Events.h"

const std::string DEFAULT_LAYER_NAME = "Layer";

namespace Karen
{
  class KAREN_API Layer
  {
  public:
    Layer(const std::string& name = DEFAULT_LAYER_NAME);
    virtual ~Layer();

    virtual void onAttach()      {}
    virtual void onDetach()     {}
    virtual void onUpdate()      {}
    virtual void onEvent(Event&) {}
    virtual void onGuiUpdate()   {}
  
    inline const std::string& getName() const {return m_name;}
  
  protected:
    std::string m_name;
  };
}
#endif //LAYER_H
