#include "Platforms/OpenGl/OpenGlRendererAPI.h"
#include "Platforms/OpenGl/OpenGlCore.h"


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
