#ifndef KR_UUID_H
#define KR_UUID_H

#include "Real-Engine/Core/Core.h"
#include <vector> //for hash()


namespace Real
{
  class REAL_API UUID 
  {
  public:
    UUID();
    UUID(uint64_t);
    UUID(const UUID&) = default;

    operator uint64_t() const { return m_id; }
  public:
    constexpr static const uint64_t invalid = 0;
  private:
    uint64_t gen(uint8_t fault);
  private:
    uint64_t m_id;
  };
}

namespace std
{
  template<>
  struct hash<Real::UUID>
  {
    size_t operator()(const Real::UUID& uuid) const
    {
      return hash<uint64_t>()((uint64_t)uuid);
    }
  };
}



#endif //KR_UUID_H
