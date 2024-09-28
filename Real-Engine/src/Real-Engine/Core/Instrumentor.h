#ifndef INSTRUMENTOR_H
#define INSTRUMENTOR_H

#include "Core.h"
#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>

namespace Real
{
  struct ProfileResult
  {
    std::string Name;
    long long Start, End;
    uint32_t ThreadID;
  };

  struct InstrumentationSession
  {
    std::string Name;
  };

  class Instrumentor
  {
  private:
    InstrumentationSession* m_current_session;
    std::ofstream m_output_stream;
    uint32_t m_profile_count;
    static bool s_should_work;
  public:
    Instrumentor()
        : m_current_session(nullptr), m_profile_count(0)
    {
    }

    static void work(bool work)
    {
      s_should_work = work;
    }

    void beginSession(const std::string& name, const std::string& filepath = "results.json")
    {
        m_output_stream.open(filepath);
        writeHeader();
        m_current_session = new InstrumentationSession{ name };
    }

    void endSession()
    {
        writeFooter();
        m_output_stream.close();
        delete m_current_session;
        m_current_session = nullptr;
        m_profile_count = 0;
    }

    void writeProfile(const ProfileResult& result)
    {
      if(s_should_work)
      {
        if (m_profile_count++ > 0)
          m_output_stream << ",";
      
        std::string name = result.Name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_output_stream << "{";
        m_output_stream << "\"cat\":\"function\",";
        m_output_stream << "\"dur\":" << (result.End - result.Start) << ',';
        m_output_stream << "\"name\":\"" << name << "\",";
        m_output_stream << "\"ph\":\"X\",";
        m_output_stream << "\"pid\":0,";
        m_output_stream << "\"tid\":" << result.ThreadID << ",";
        m_output_stream << "\"ts\":" << result.Start;
        m_output_stream << "}";

        m_output_stream.flush();
      }
    }

    void writeHeader()
    {
        m_output_stream << "{\"otherData\": {},\"traceEvents\":[";
        m_output_stream.flush();
    }

    void writeFooter()
    {
        m_output_stream << "]}";
        m_output_stream.flush();
    }

    static Instrumentor& get()
    {
        static Instrumentor instance;
        return instance;
    }
  };

  class InstrumentationTimer
  {
  public:
    InstrumentationTimer(const char* name)
        : m_name(name), m_stopped(false)
    {
        m_start_timepoint = std::chrono::high_resolution_clock::now();
    }
    
    ~InstrumentationTimer()
    {
        if (!m_stopped)
            stop();
    }

    void stop()
    {
        auto end_timepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start_timepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(end_timepoint).time_since_epoch().count();
        uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
        Instrumentor::get().writeProfile({ m_name, start, end, threadID });

        m_stopped = true;
    }
  private:
    const char* m_name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start_timepoint;
    bool m_stopped;
  };
}
#define REAL_ENABLE_INSTRUMENTATION
#define REAL_FUSE(x, y) x ## y
#define FUSE(x, y) REAL_FUSE(x, y)
  #ifdef REAL_ENABLE_INSTRUMENTATION
    #define REAL_START_INSTRUMENTOR() Real::Instrumentor::get().work(1)
    #define REAL_BEGIN_PROFILE(name, file_path) Real::Instrumentor::get().beginSession(name, file_path)
    #define REAL_PROFILE_SCOPE(name) Real::InstrumentationTimer FUSE(timer, __LINE__) (name)
    #define REAL_PROFILE_FUNCTION() REAL_PROFILE_SCOPE(__REAL_FUNCTION__)
    #define REAL_END_PROFILE() Real::Instrumentor::get().endSession()
    #define REAL_STOP_INSTRUMENTOR() Real::Instrumentor::get().work(0)
  
  #else
    #define REAL_START_INSTRUMENTOR
    #define REAL_BEGIN_PROFILE(name, file_path)
    #define REAL_PROFILE_SCOPE(name)
    #define REAL_PROFILE_FUNCTION()
    #define REAL_END_PROFILE()
    #define REAL_STOP_INSTRUMENTOR
  #endif //REAL_ENABLE_INSTRUMENTATION
#endif //INSTRUMENTOR_H
