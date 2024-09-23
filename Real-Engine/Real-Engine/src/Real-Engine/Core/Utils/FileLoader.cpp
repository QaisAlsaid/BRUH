#include "pch.h"
#include "FileLoader.h"

namespace Karen
{
  std::string FileLoader::LoadFromFile(const std::string& fn)
  {
    std::ifstream fs(fn);
    std::string line;
    std::stringstream ss;
    if(!fs)
    {
      KAREN_CORE_ASSERT_MSG(false, std::string("Could Not Open File: ") + fn);
      return "";
    }

    while(std::getline(fs, line))
    {
      ss << line << "\n";
    }
    return ss.str();
  }
}
