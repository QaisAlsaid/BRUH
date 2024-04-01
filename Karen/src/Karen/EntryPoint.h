#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H

#include "Core.h"
#include "App.h"
#include "Log.h"
#include <iostream>

Karen::App* Karen::createApp();

int main(int argc, char* argv[])
{
  Karen::Log::init();
  Karen::Log::setCoreLogLevel(Karen::Log::LogLevel::Trace);
  Karen::App* app = Karen::createApp();
  app->run();
}

#endif //ENTRY_POINT_H
