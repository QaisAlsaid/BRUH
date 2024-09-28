#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include "Real-Engine/Core/Core.h"
#include <string>

namespace Real
{
  class REAL_API FileLoader
  {
  public:
    static std::string LoadFromFile(const std::string& file_name);
  };
}

#endif //FILE_LOADER_H
