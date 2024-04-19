#ifndef CORE_H
#define CORE_H

#include <memory>
#include "Assertion.h"
#include "Macros.h"


#ifdef KAREN_IS_SHARED
#if defined(_MSC_VER)
  #ifdef KAREN_BUILD_SHARDE 
    #define KAREN_API __declspec(dllexport)
  #else
    #define KAREN_API __declspec(dllimport)
  #endif //KAREN_BUILD_SHARDE
#elif defined(__GNUC__)
  #ifdef KAREN_BUILD_SHARDE
    #define KAREN_API __attribute__((visibility("default")))
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

namespace Karen
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
