#ifndef OPENGL_CONTEXT_H
#define OPENGL_CONTEXT_H

#include "Karen/Render/API/Context.h"

struct GLFWwindow;

namespace Karen
{
  class OpenGlContext : public Context
  {
  public:
    OpenGlContext(GLFWwindow* window);

    void init()        override;
    void swapBuffers() override;
  private:
    GLFWwindow* m_window;
  };
}

#endif //OPENGL_CONTEXT_H
