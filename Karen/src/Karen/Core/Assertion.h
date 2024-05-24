#ifndef ASSERTION_H
#define ASSERTION_H

#include "Macros.h"
#include <exception>

#ifdef KAREN_ENABLE_ASSERT
  #define KAREN_CORE_ASSERT_MSG(x, ...) \
                                        if(!(x))\
                                        {\
                                          KAREN_CORE_CRITICAL("{0}", __VA_ARGS__);\
                                          abort();\
                                        }
  #define KAREN_ASSERT_MSG(x, ...) \
                                        if(!(x)) \
                                        {\
                                          KAREN_CRITICAL("{0}", __VA_ARGS__);\
                                          abort();\
                                        }
  #define KAREN_CORE_ASSERT(x) \
                                        if(!(x))\
                                        {\
                                          KAREN_CORE_CRITICAL("Assertion Raised {0}, {1}, {2}", __FILE__, __LINE__, __KAREN_FUNCTION__);\
                                          abort();\
                                        }
  #define KAREN_ASSERT(x) \
                                        if(!(x))\
                                        {\
                                          KAREN_CRITICAL("Assertion Raised {0}, {1}, {2}", __FILE__, __LINE__, __KAREN_FUNCTION__);\
                                          abort();\
                                        }

#else
  #define KAREN_CORE_ASSERT(x, ...)
  #define KAREN_ASSERT(x, ...)
#endif //KAREN_ENABLE_ASSERT
#endif //ASSERTION_H
