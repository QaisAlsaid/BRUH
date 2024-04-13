#include <Karen/EntryPoint.h>
#include <Karen/Karen.h>

//#include "Sandbox.h"
#include "Sandbox2D.h"

class ClientApp : public Karen::App
{
public:
  ClientApp(const std::string& s)
  {
    KAREN_INFO("APP Created");
    KAREN_INFO("{0}", s);
    //pushLayer(new SandboxLayer());
    pushLayer(new Sandbox2DLayer());
  }
  ~ClientApp()
  {

  }
};

Karen::App* Karen::createApp()
{
  KAREN_SET_LOGLEVEL(Karen::Log::LogLevel::Trace);
  return new ClientApp("Constructed");
}
