#ifndef CONTEXT_H
#define CONTEXT_H

#include "Real-Engine/Core/Core.h"


namespace Real 
{
  class Context
  {
  public:
    virtual void init()        = 0;
    virtual void swapBuffers() = 0;
  };
}

#endif //CONTEXT_H
