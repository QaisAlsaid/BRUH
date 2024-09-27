#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H

#include "Real-Engine/Core/Core.h"
#include "Real-Engine/Core/App.h"
#include "Real-Engine/Core/Log.h"
#include "Real-Engine/Core/Instrumentor.h"


Real::App* Real::createApp();

int main(int argc, char* argv[])
{
  REAL_BEGIN_PROFILE("Startup", "Real-EngineProfile-Startup.json");
  
  Real::Log::init();
  REAL_CORE_SET_LOGLEVEL(Real::Log::LogLevel::Trace);
  Real::App* app = Real::createApp();
  
  REAL_END_PROFILE();
  
  REAL_BEGIN_PROFILE("Runtime", "Real-EngineProfile-Runtime.json");
  
  app->run();
  
  REAL_END_PROFILE();
  
  REAL_BEGIN_PROFILE("Shutdown", "Real-EngineProfile-Shutdown");
  
  delete app;
  
  REAL_END_PROFILE();

}

#endif //ENTRY_POINT_H
