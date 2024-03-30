#include <Karen.h>

class mLayer : public Karen::Layer
{
public:
  mLayer() : Layer("Test") {}
  void onAttach()        override {KAREN_INFO("Layer: {0} Attached", this->getName());}
  void onDetach()       override {KAREN_INFO("Layer: {0} Detached", this->getName());}
  void onUpdate()        override {if(Karen::Input::isKeyPressed(Karen::Keyboard::Q)) KAREN_TRACE("Q");}
  void onEvent(Karen::Event& e) override
  {
    if(e.isInCategory(Karen::EventCategory::CATMouse))
    {
      KAREN_TRACE("Layer: {0} Received an Event: {1}", this->getName(), e);
      e.setHandled();
    }
  }
};
class Sandbox : public Karen::App
{
public:
  Sandbox()
  {
    pushLayer(new mLayer());
  }
  ~Sandbox()
  {

  }
};

Karen::App* Karen::createApp()
{
  KAREN_SET_LOGLEVEL(Karen::Log::LogLevel::Trace);
  return new Sandbox();
}
