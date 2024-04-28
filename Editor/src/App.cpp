#include <Karen/Core/EntryPoint.h>
#include <Karen/Karen.h>

#include "EditorLayer.h"

namespace Karen
{

  class EditorApp : public App
  {
  public:
    EditorApp() 
    {
      pushLayer(new EditorLayer());
    }
    ~EditorApp()
    {

    }
  };

  App* createApp()
  {
    KAREN_SET_LOGLEVEL(Log::LogLevel::Trace);
    return new EditorApp();
  }
}
