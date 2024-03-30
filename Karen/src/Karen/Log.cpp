#include "pch.h"
#include "Karen/Log.h"

namespace Karen
{
  std::shared_ptr<spdlog::logger> Log::stat_core_logger;
  std::shared_ptr<spdlog::logger> Log::stat_clinet_logger;
  void Log::init()
  { 
    spdlog::set_pattern("%^@[%T] %@ %n: %v %$");
    
    stat_core_logger = spdlog::stdout_color_mt("KAREN");
    stat_core_logger->set_level((spdlog::level::level_enum) Log::LogLevel::Warn);
  

    stat_clinet_logger = spdlog::stdout_color_mt("APP");
    stat_clinet_logger->set_level((spdlog::level::level_enum) Log::LogLevel::Trace);
  }
}
