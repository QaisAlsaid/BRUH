#ifndef OPENGL_RENDERER_API_H
#define OPENGL_RENDERER_API_H

#include "Karen/Render/API/RendererAPI.h"


namespace Karen 
{
  class KAREN_API OpenGLRendererAPI : public RendererAPI
  {
  public:
    OpenGLRendererAPI()  = default;
    ~OpenGLRendererAPI() = default;
    void init() override;
    void clear(const Vec4& p_clear_color) override;
    void drawIndexed(const ARef<VertexArray>& p_varr) override;
  };
}

#endif //OPENGL_RENDERER_API_H
