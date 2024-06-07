#ifndef GUI_LAYER_H
#define GUI_LAYER_H

#include "Karen/Core/Core.h"
#include "Karen/Core/Window.h"
#include "Karen/Core/Layers/Layer.h"

namespace Karen
{
  class KAREN_API GuiLayer : public Layer
  {
  public:
    GuiLayer(const std::string& name = DEFAULT_LAYER_NAME);
    virtual ~GuiLayer();
    virtual void onAttach()      override;
    virtual void onDetach()      override;
    virtual void onGuiUpdate()   override;
    void         begin();
    void         end();
  private:
    float   m_time = 0.0f;
  private:
    void init();
  };
}
#endif //GUI_LAYER_H
