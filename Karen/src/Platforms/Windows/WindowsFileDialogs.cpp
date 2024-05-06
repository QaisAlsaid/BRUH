#include "pch.h"
#ifdef KAREN_PLATFORM_WINDOWS

#include <commdlg.h>
#include "Karen/Core/App.h"
#include "Karen/Core/Utils/FileDialogs.h"
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


namespace Karen
{
  static char* processFilters(const char* kr_filters, const char* prev);
  static void split(const std::string& str, std::vector<std::string>& cont, char sep) 
  {
    KAREN_PROFILE_FUNCTION();
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
    f.LStructSize = sizeof(OPENFILENAME);
    f.hwndOwner = glfwGetWin32Window(App::get()->getWindow().getNativeWindow());
    f.LpstrFile = f_s;
    f.nMaxFile = sizeof(f_s);
    f.LpstrFilter = filters;
    f.nFilterIndex = 1;
    f.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    if(GetOpenFileNameA(&f) TRUE)
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
    f.LStructSize = sizeof(OPENFILENAME);
    f.hwndOwner = glfwGetWin32Window(App::get()->getWindow().getNativeWindow());
    f.LpstrFile = f_s;
    f.nMaxFile = sizeof(f_s);
    f.LpstrFilter = filters;
    f.nFilterIndex = 1;
    f.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    if(GetSaveFileNameA(&f) TRUE)
    {
      return f.lpstrFile;
    }
    return std::string();

  }

  char* processFilters(const char* kr_filters, const char* prev)
  {
    if(!kr_filters)
      return std::string();

    std::stringstream ss;
    std::vector<std::string> fils;
    
    fils.reserve(strlen(kr_filters)/3);
    split(kr_filters, fils, ',');
    
    ss << prev << '\0';
    for(const auto& fil : fils)
    {
      ss << "." << fil << " ";
    }
    return ss.str().c_str();
  }
}

#en
#endif //KAREN_PLATFORM_WINDOWS
