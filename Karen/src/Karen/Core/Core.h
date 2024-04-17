#ifndef CORE_H
#define CORE_H

#include <memory>
#include <utility>

#include "Assertion.h"

#ifdef KAREN_IS_SHARED
#if defined(_MSC_VER)
  #ifdef KAREN_BUILD_SHARDE 
    #define KAREN_API __declspec(dllexport)
  #else
    #define KAREN_API __declspec(dllimport)
  #endif //KAREN_BUILD_SHARDE
#elif defined(__GNUC__)
  #ifdef KAREN_BUILD_SHARDE
    #define KAREN_API /*__attribute__((visibility("default")))*/
  #else
    #define KAREN_API
  #endif //KAREN_BUILD_SHARDE
#else
  #define KAREN_API
  #pragma warning Unknown dynamic link import/export semantics.
#endif //_MSC_VER
#else
  #define KAREN_API
#endif //KAREN_IS_SHARED

#define BITSHL(x) (1 << x)
#define BIND_EVENT_FUNCTION(x) std::bind(&x, this, std::placeholders::_1)

namespace Karen
{
  template<typename T>
  using ARef = std::shared_ptr<T>;
  template<typename T>
  using Scoped = std::unique_ptr<T>;
}

#endif //CORE_H
