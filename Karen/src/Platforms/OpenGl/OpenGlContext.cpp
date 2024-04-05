#include "pch.h"
#include "Platforms/OpenGl/OpenGlContext.h"

#ifndef KAREN_EMSCRIPTEN
  #include <glad/glad.h>
#else
  #include <glad/gles2.h>
#endif //KAREN_EMSCRIPTEN
#include <GLFW/glfw3.h>

namespace Karen
{
  OpenGlContext::OpenGlContext(GLFWwindow* window)
  : m_window(window)
  {
  }

  void OpenGlContext::init()
  {
    glfwMakeContextCurrent(m_window);
#ifndef KAREN_EMSCRIPTEN
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    KAREN_CORE_ASSERT(status, "ERROR Loading OpenGl Function Pointers");
#else 
    int status = gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);
    KAREN_CORE_ASSERT(status, "ERROR Loading OpenGl Function Pointers");
#endif //KAREN_EMSCRIPTEN
  }
  
  void OpenGlContext::swapBuffers()
  {
    glfwSwapBuffers(m_window);
  }
}
