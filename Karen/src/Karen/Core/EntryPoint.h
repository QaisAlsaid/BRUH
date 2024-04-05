#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H

#include "Karen/Core/Core.h"
#include "Karen/Core/App.h"
#include "Karen/Core/Log.h"

Karen::App* Karen::createApp();

int main(int argc, char* argv[])
{
  Karen::Log::init();
  Karen::Log::setCoreLogLevel(Karen::Log::LogLevel::Trace);
  Karen::App* app = Karen::createApp();
  app->run();
}

#endif //ENTRY_POINT_H
