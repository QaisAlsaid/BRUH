#ifndef CONTEXT_H
#define CONTEXT_H

#include "Karen/Core/Core.h"


namespace Karen 
{
  class Context
  {
  public:
    virtual void init()        = 0;
    virtual void swapBuffers() = 0;
  };
}

#endif //CONTEXT_H
