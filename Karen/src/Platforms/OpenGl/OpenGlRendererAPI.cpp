#include "Platforms/OpenGl/OpenGlRendererAPI.h"

#ifndef KAREN_EMSCRIPTEN
#include <glad/glad.h>
#else 
#include <GLES3/gl32.h>

#endif //KAREN_EMSCRIPTEN

namespace Karen
{
  void OpenGLRendererAPI::drawIndexed(const ARef<VertexArray>& p_varr)
  { 
    p_varr->bind();
    glDrawElements(GL_TRIANGLES, p_varr->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
  }

  void OpenGLRendererAPI::clear(const Vec4& col)
  {
    glClearColor(col.x, col.y, col.z, col.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
}
