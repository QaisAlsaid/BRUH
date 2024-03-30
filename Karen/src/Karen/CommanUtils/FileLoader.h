#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include "Karen/Core.h"
#include <string>

namespace Karen
{
  class KAREN_API FileLoader
  {
  public:
    static std::string LoadFromFile(const std::string& file_name);
  };
}

#endif //FILE_LOADER_H
