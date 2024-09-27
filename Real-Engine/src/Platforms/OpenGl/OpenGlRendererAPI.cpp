#include "Platforms/OpenGl/OpenGlRendererAPI.h"
#include "Platforms/OpenGl/OpenGlCore.h"


namespace Real
{
  void OpenGLRendererAPI::init()
  {
    glCall(glEnable(GL_DEPTH_TEST));
    glCall(glEnable(GL_BLEND));
    glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    //glCall(glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE));
  }

  void OpenGLRendererAPI::setViewPort(uint32_t left, uint32_t bottom, uint32_t right, uint32_t top)
  {
    REAL_PROFILE_FUNCTION();
    glCall(glViewport(left, bottom, right, top));
  }

  void OpenGLRendererAPI::drawIndexed(const ARef<VertexArray>& p_varr, uint32_t p_index_count)
  { 
    REAL_PROFILE_FUNCTION();
    const uint32_t count = p_index_count == 0 ? p_varr->getIndexBuffer()->getCount() : p_index_count;
    p_varr->bind();
    glCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
  }

  void OpenGLRendererAPI::clear(const Vec4& col)
  {
    REAL_PROFILE_FUNCTION();
    glCall(glClearColor(col.x, col.y, col.z, col.w));
    glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  }
}
