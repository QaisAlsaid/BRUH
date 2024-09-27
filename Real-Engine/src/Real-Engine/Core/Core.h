#ifndef CORE_H
#define CORE_H

#include <memory>
#include "Assertion.h"
#include "Macros.h"


#ifdef REAL_IS_SHARED
#if defined(_MSC_VER)
  #ifdef REAL_BUILD_SHARDE 
    #define REAL_API __declspec(dllexport)
  #else
    #define REAL_API __declspec(dllimport)
  #endif //REAL_BUILD_SHARDE
#elif defined(__GNUC__)
  #ifdef REAL_BUILD_SHARDE
    #define REAL_API __attribute__((visibility("default")))
  #else
    #define REAL_API
  #endif //REAL_BUILD_SHARDE
#else
  #define REAL_API
  #pragma warning Unknown dynamic link import/export semantics.
#endif //_MSC_VER
#else
  #define REAL_API
#endif //REAL_IS_SHARED

namespace Real
{
  template<typename T>
	using Scoped = std::unique_ptr<T>;
	template<typename T, typename... Args>
	constexpr Scoped<T> createScoped(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}


	template<typename T>
	using ARef = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr ARef<T> createARef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#endif //CORE_H
