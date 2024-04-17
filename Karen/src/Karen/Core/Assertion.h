#ifndef ASSERTION_H
#define ASSERTION_H


#ifdef KAREN_ENABLE_ASSERT
  #define KAREN_CORE_ASSERT(x, ...) \
    if(!(x))\
    {\
      KAREN_CORE_CRITICAL("{0}", __VA_ARGS__);\
      abort();\
    }
  #define KAREN_ASSERT(x, ...) if(!(x)) \
                              {\
                                KAREN_CRITICAL("{0}", __VA_ARGS__);\
                                aboart();\
                              }
#else
  #define KAREN_CORE_ASSERT(x, ...)
  #define KAREN_ASSERT(x, ...)
#endif //KAREN_ENABLE_ASSERT
#endif //ASSERTION_H
