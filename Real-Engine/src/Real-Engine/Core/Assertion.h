#ifndef ASSERTION_H
#define ASSERTION_H

#include "Macros.h"
#include <exception>

#ifdef REAL_ENABLE_ASSERT
  #define REAL_CORE_ASSERT_MSG(x, ...) \
                                        if(!(x))\
                                        {\
                                          REAL_CORE_CRITICAL("{0}", __VA_ARGS__);\
                                          abort();\
                                        }
  #define REAL_ASSERT_MSG(x, ...) \
                                        if(!(x)) \
                                        {\
                                          REAL_CRITICAL("{0}", __VA_ARGS__);\
                                          abort();\
                                        }
  #define REAL_CORE_ASSERT(x) \
                                        if(!(x))\
                                        {\
                                          REAL_CORE_CRITICAL("Assertion Raised {0}, {1}, {2}", __FILE__, __LINE__, __REAL_FUNCTION__);\
                                          abort();\
                                        }
  #define REAL_ASSERT(x) \
                                        if(!(x))\
                                        {\
                                          REAL_CRITICAL("Assertion Raised {0}, {1}, {2}", __FILE__, __LINE__, __REAL_FUNCTION__);\
                                          abort();\
                                        }

#else
  #define REAL_CORE_ASSERT(x, ...)
  #define REAL_ASSERT(x, ...)
#endif //REAL_ENABLE_ASSERT
#endif //ASSERTION_H
