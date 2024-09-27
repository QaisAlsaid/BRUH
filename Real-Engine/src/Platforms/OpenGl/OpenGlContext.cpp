#include "pch.h"
#include "Platforms/OpenGl/OpenGlContext.h"

#include "Platforms/OpenGl/OpenGlCore.h"

#ifndef REAL_EMSCRIPTEN
  #include <glad/glad.h>
#else 
  #include<emscripten.h>
  #include<emscripten/html5.h>
  #include <GLES3/gl32.h>
#endif //REAL_EMSCRIPTEN
#include <GLFW/glfw3.h>



namespace Real
{
  OpenGlContext::OpenGlContext(GLFWwindow* window)
  : m_window(window)
  {
  }

  void OpenGlContext::init()
  {
    REAL_PROFILE_FUNCTION();
#ifndef REAL_EMSCRIPTEN 
    glfwMakeContextCurrent(m_window);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    REAL_CORE_ASSERT_MSG(status, "ERROR Loading OpenGl Function Pointers");
#else 
  EmscriptenWebGLContextAttributes attrs;
    attrs.antialias = true;
    attrs.majorVersion = 3;
    attrs.minorVersion = 2;
    attrs.alpha = true;
    attrs.powerPreference = EM_WEBGL_POWER_PREFERENCE_DEFAULT;

    // The following lines must be done in exact order, or it will break!
    emscripten_webgl_init_context_attributes(&attrs); // you MUST init the attributes before creating the context
    attrs.majorVersion = 3; // you MUST set the version AFTER the above line
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE webgl_context = emscripten_webgl_create_context("#canvas", &attrs);
    emscripten_webgl_make_context_current(webgl_context);
  glfwMakeContextCurrent(m_window);
#endif //REAL_EMSCRIPTEN
  }
  
  void OpenGlContext::swapBuffers()
  {
    REAL_PROFILE_FUNCTION();
    glfwSwapBuffers(m_window);
  }
}
