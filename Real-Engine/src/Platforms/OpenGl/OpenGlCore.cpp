#include "pch.h"
#include "Platforms/OpenGl/OpenGlCore.h"


void glPrintErr(const char* fn, const char* file, int line)
{
  REAL_PROFILE_FUNCTION();
  while(auto err = glGetError())
  {
    REAL_CORE_CRITICAL("OpenGl Error code {0}, File: {1}, Line: {2}, function: {3}",err, file, line, fn);
  }
}
