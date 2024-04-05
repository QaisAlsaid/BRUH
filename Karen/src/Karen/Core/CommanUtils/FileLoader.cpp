#include "pch.h"
#include "Karen/Core/CommanUtils/FileLoader.h"


namespace Karen
{
  std::string FileLoader::LoadFromFile(const std::string& fn)
  {
    std::ifstream fs(fn);
    std::string line;
    std::stringstream ss;
    if(!fs)
    {
      KAREN_CORE_ASSERT(false, std::string("Could Not Open File: ") + fn);
      return "Error Openning File:";
    }

    while(std::getline(fs, line))
    {
      ss << line << "\n";
    }
    return ss.str();
  }
}
