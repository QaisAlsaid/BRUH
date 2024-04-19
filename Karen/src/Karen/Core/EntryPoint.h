#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H

#include "Karen/Core/Core.h"
#include "Karen/Core/App.h"
#include "Karen/Core/Log.h"
#include "Karen/Core/Instrumentor.h"


Karen::App* Karen::createApp();

int main(int argc, char* argv[])
{
  KAREN_BEGIN_PROFILE("Startup", "KarenProfile-Startup.json");
  
  Karen::Log::init();
  KAREN_CORE_SET_LOGLEVEL(Karen::Log::LogLevel::Trace);
  Karen::App* app = Karen::createApp();
  
  KAREN_END_PROFILE();
  
  KAREN_BEGIN_PROFILE("Runtime", "KarenProfile-Runtime.json");
  
  app->run();
  
  KAREN_END_PROFILE();
  
  KAREN_BEGIN_PROFILE("Shutdown", "KarenProfile-Shutdown");
  
  delete app;
  
  KAREN_END_PROFILE();

}

#endif //ENTRY_POINT_H
