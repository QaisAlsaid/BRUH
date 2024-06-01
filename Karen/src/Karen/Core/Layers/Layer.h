#ifndef LAYER_H
#define LAYER_H


#include "Karen/Core/Core.h"
#include "Karen/Core/Events/Events.h"
#include "Karen/Core/Timestep.h"
#include "Karen/Core/Math/math.h"

const std::string DEFAULT_LAYER_NAME = "Layer";

namespace Karen
{
  class KAREN_API Layer
  {
  public:
    Layer(const std::string& name = DEFAULT_LAYER_NAME);
    virtual ~Layer();

    virtual void onAttach()                {}
    virtual void onDetach()                {}
    virtual void onUpdate(Timestep t)      {}
    virtual void onEvent(Event&)           {}
    virtual void onGuiUpdate()             {}

    inline void setName(const std::string& name) {this->name = name;}
    inline const std::string& getName() const {return this->name;}
    inline void setActive(bool active) {this->active = active;}
    inline bool isActive() const {return active;}
    inline void setGuiActive(bool active) {gui_active = active;}
    inline bool isGuiActive() const {return gui_active;}
    inline void activate() {active = true; gui_active = true;}

  protected:
    std::string name;
    bool active  = false;
    bool gui_active = false;
  };
}
#endif //LAYER_H
