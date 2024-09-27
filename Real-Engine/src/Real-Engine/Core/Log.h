#ifndef LOG_H
#define LOG_H

#include "Real-Engine/Core/Core.h"
#include <spdlog/spdlog.h>
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Real
{
  class REAL_API Log
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
    template<typename... Args>
    static inline void trace(Args&&... arg)
    {
      stat_clinet_logger->trace(std::forward<Args>(arg)...);
    }
    template<typename... Args>
    static inline void info(Args&&... arg)
    {
      stat_clinet_logger->info(std::forward<Args>(arg)...);
    }
    template<typename... Args>
    static inline void warn(Args&&... arg)
    {
      stat_clinet_logger->warn(std::forward<Args>(arg)...);
    }
    template<typename... Args>
    static inline void error(Args&&... arg)
    {
      stat_clinet_logger->error(std::forward<Args>(arg)...);
    }

    template<typename... Args>
    static inline void critical(Args&&... arg)
    {
      stat_clinet_logger->critical(std::forward<Args>(arg)...);
    }


  private:
    static std::shared_ptr<spdlog::logger> stat_clinet_logger;
    static std::shared_ptr<spdlog::logger> stat_core_logger;
  };
}
//clinet logger
#define REAL_TRACE(...) ::Real::Log::getClinetLoger()->trace(__VA_ARGS__)
#define REAL_INFO(...) ::Real::Log::getClinetLoger()->info(__VA_ARGS__)
#define REAL_WARN(...) ::Real::Log::getClinetLoger()->warn(__VA_ARGS__)
#define REAL_ERROR(...) ::Real::Log::getClinetLoger()->error(__VA_ARGS__)
#define REAL_CRITICAL(...) ::Real::Log::getClinetLoger()->critical(__VA_ARGS__)
#define REAL_SET_LOGLEVEL(x) ::Real::Log::setClinetLogLevel(x)
//core logger
#define REAL_CORE_TRACE(...) ::Real::Log::getCoreLoger()->trace(__VA_ARGS__)
#define REAL_CORE_INFO(...) ::Real::Log::getCoreLoger()->info(__VA_ARGS__)
#define REAL_CORE_WARN(...) ::Real::Log::getCoreLoger()->warn(__VA_ARGS__)
#define REAL_CORE_ERROR(...) ::Real::Log::getCoreLoger()->error(__VA_ARGS__)
#define REAL_CORE_CRITICAL(...) ::Real::Log::getCoreLoger()->critical(__VA_ARGS__)
#define REAL_CORE_SET_LOGLEVEL(x) ::Real::Log::setCoreLogLevel(x)



#endif //LOG_H
