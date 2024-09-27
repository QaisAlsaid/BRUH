#include "pch.h"
#ifdef REAL_PLATFORM_WINDOWS

#include <commdlg.h>
#include "Real-Engine/Core/App.h"
#include "Real-Engine/Core/Utils/FileDialogs.h"
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


namespace Real
{
  static const char* processFilters(const char* kr_filters, const char* prev);
  static void split(const std::string& str, std::vector<std::string>& cont, char sep) 
  {
    REAL_PROFILE_FUNCTION();
    std::stringstream ss(str);
    std::string token;
    while(ss >> token)
    {
      token.erase(std::remove(token.begin(), token.end(), sep), token.end());
      cont.push_back(token);
    }
  }

  std::string FileDialogs::OpenFile(const char* kr_filters, const char* prev)
  {
    auto* filters = processFilters(kr_filters, prev);
    OPENFILENAME f;
    CHAR f_s[1024] = {0};
    ZeroMemory(&f, sizeof(OPENFILENAME));
    f.lStructSize = sizeof(OPENFILENAME);
    f.hwndOwner = glfwGetWin32Window((GLFWwindow*)App::get()->getWindow().getNativeWindow());
    f.lpstrFile = f_s;
    f.nMaxFile = sizeof(f_s);
    f.lpstrFilter = filters;
    f.nFilterIndex = 1;
    f.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    if(GetOpenFileName(&f) == TRUE)
    {
      return f.lpstrFile;
    }
    return std::string();
   }

  std::string FileDialogs::SaveFile(const char* kr_filters, const char* prev)
  {
    auto* filters = processFilters(kr_filters, prev);
    OPENFILENAME f;
    CHAR f_s[1024] = {0};
    ZeroMemory(&f, sizeof(OPENFILENAME));
    f.lStructSize = sizeof(OPENFILENAME);
    f.hwndOwner = glfwGetWin32Window((GLFWwindow*)App::get()->getWindow().getNativeWindow());
    f.lpstrFile = f_s;
    f.nMaxFile = sizeof(f_s);
    f.lpstrFilter = filters;
    f.nFilterIndex = 1;
    f.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    if(GetSaveFileName(&f) == TRUE)
    {
      return f.lpstrFile;
    }
    return std::string();

  }

  const char* processFilters(const char* kr_filters, const char* prev)
  {
    if(!kr_filters)
      return nullptr;

    std::stringstream ss;
    std::vector<std::string> fils;
    
    fils.reserve(strlen(kr_filters)/3);
    split(kr_filters, fils, ',');
    
    ss << prev << '\0';
    for(const auto& fil : fils)
    {
      ss << "." << fil << " ";
    }
    const auto& str = ss.str();
    char* buff = (char*)alloca(sizeof(char) * str.size());
    for(uint32_t i = 0; i < str.size(); ++i)
    {
      buff[i] = str.at(i);
    }
    return buff;
  }
}

#endif //REAL_PLATFORM_WINDOWS
