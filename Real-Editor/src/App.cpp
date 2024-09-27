#include <Real-Engine/Core/EntryPoint.h>
#include <Real-Engine/Real-Engine.h>

#include "EditorLayer.h"

namespace Real
{

  class EditorApp : public App
  {
  public:
    EditorApp() 
    {
      EditorLayer* al = new EditorLayer();
      pushLayer(al);
    }
    ~EditorApp()
    {

    }
  };

  App* createApp()
  {
    REAL_SET_LOGLEVEL(Log::LogLevel::Trace);
    return new EditorApp();
  }
}
