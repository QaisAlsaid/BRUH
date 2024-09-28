#include "pch.h"
#include <random>
#include "UUID.h"

static std::random_device s_rd;
static std::mt19937_64 s_engine(s_rd());
static std::uniform_int_distribution<uint64_t> s_ud;

namespace Real
{
  UUID::UUID()
  {
    m_id = gen(0);
  }

  UUID::UUID(uint64_t uint64)
    : m_id(uint64) 
  {
  }

  uint64_t UUID::gen(uint8_t fault)
  {
    auto id = s_ud(s_engine);
    if(id == invalid) gen(++fault);
    return id;
  }
}
