#ifndef KR_UUID_H
#define KR_UUID_H

#include "Karen/Core/Core.h"
#include <vector> //for hash()


namespace Karen
{
  class KAREN_API UUID 
  {
  public:
    UUID();
    UUID(uint64_t);
    UUID(const UUID&) = default;

    operator uint64_t() const { return m_id; }
  private:
    uint64_t m_id;
  };
}

namespace std
{
  template<>
  struct hash<Karen::UUID>
  {
    size_t operator()(const Karen::UUID& uuid) const
    {
      return hash<uint64_t>()((uint64_t)uuid);
    }
  };
}



#endif //KR_UUID_H
