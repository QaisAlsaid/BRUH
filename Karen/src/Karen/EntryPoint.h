#include "Core.h"
#include "App.h"
#include "Log.h"
#include <iostream>
#ifdef __linux

Karen::App* Karen::createApp();

int main(int argc, char* argv[])
{
  Karen::Log::init();
  Karen::Log::setCoreLogLevel(Karen::Log::LogLevel::Trace);
  Karen::App* app = Karen::createApp();
  app->run();
}

#endif //__linux
