#include "pch.h"
#include "Platforms/OpenGl/OpenGlContext.h"

#include <glad/glad.h>
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
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    KAREN_CORE_ASSERT(status, "ERROR Loading OpenGl Function Pointers");
  }
  
  void OpenGlContext::swapBuffers()
  {
    glfwSwapBuffers(m_window);
  }
}
