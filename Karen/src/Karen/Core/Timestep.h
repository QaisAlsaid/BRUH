#ifndef TIMESTEP_H
#define TIMESTEP_H

#include "Karen/Core/Core.h"


namespace Karen
{
  class KAREN_API Timestep
  {
  public:
    Timestep() = default;
    Timestep(float current_time)
    {
      m_time = current_time;
    }

    inline float getTime() const
    {
      return m_time;
    }

    inline float getTimeAsMilliSeconds() const
    {
      return m_time * 1000.0f;
    }

    operator float() 
    {
      return m_time;
    }

  private:
    float m_time = 0.0f;
  };
}

#endif //TIMESTEP_H
