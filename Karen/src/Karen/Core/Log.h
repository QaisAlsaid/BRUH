#ifndef LOG_H
#define LOG_H

#include "Karen/Core/Core.h"
#include <spdlog/spdlog.h>
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Karen
{
  class KAREN_API Log
  {
  public:
    enum LogLevel{
      Trace    = 0,
      Debug    = 1,
      Info     = 2,
      Warn     = 3,
      Error    = 4,
      Critical = 5,
      Off      = 6
    };
    static void init();
    static inline std::shared_ptr<spdlog::logger>& getCoreLoger()
    {
      return stat_core_logger;
    }
    static inline std::shared_ptr<spdlog::logger>& getClinetLoger()
    {
      return stat_clinet_logger;
    }
    static inline void setCoreLogLevel(LogLevel lvl)
    {
      stat_core_logger->set_level((spdlog::level::level_enum) lvl);
    }
    static inline void setClinetLogLevel(LogLevel lvl)
    {
      stat_core_logger->set_level((spdlog::level::level_enum) lvl);
    }

  private:
    static std::shared_ptr<spdlog::logger> stat_clinet_logger;
    static std::shared_ptr<spdlog::logger> stat_core_logger;
  };
}
//clinet logger
#define KAREN_TRACE(...) ::Karen::Log::getClinetLoger()->trace(__VA_ARGS__)
#define KAREN_INFO(...) ::Karen::Log::getClinetLoger()->info(__VA_ARGS__)
#define KAREN_WARN(...) ::Karen::Log::getClinetLoger()->warn(__VA_ARGS__)
#define KAREN_ERROR(...) ::Karen::Log::getClinetLoger()->error(__VA_ARGS__)
#define KAREN_CRITICAL(...) ::Karen::Log::getClinetLoger()->critical(__VA_ARGS__)
#define KAREN_SET_LOGLEVEL(x) ::Karen::Log::setClinetLogLevel(x)
//core logger
#define KAREN_CORE_TRACE(...) ::Karen::Log::getCoreLoger()->trace(__VA_ARGS__)
#define KAREN_CORE_INFO(...) ::Karen::Log::getCoreLoger()->info(__VA_ARGS__)
#define KAREN_CORE_WARN(...) ::Karen::Log::getCoreLoger()->warn(__VA_ARGS__)
#define KAREN_CORE_ERROR(...) ::Karen::Log::getCoreLoger()->error(__VA_ARGS__)
#define KAREN_CORE_CRITICAL(...) ::Karen::Log::getCoreLoger()->critical(__VA_ARGS__)
#define KAREN_CORE_SET_LOGLEVEL(x) ::Karen::Log::setCoreLogLevel(x)



#endif //LOG_H
