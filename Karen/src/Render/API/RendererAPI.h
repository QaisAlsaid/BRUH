#ifndef RENDERER_H
#define RENDERER_H


#include "Karen/Core.h"
#include "Karen/Math/math.h"
#include "Render/API/VertexArray.h"
#include <stdint.h>

namespace Karen
{
  class KAREN_API RendererAPI
  {
  public:
    enum class API : uint8_t
    {
     NONE = 0, OpenGl = 1
    };
  public:
    virtual ~RendererAPI() {}
    virtual void clear(const Vec4& p_clear_color) = 0;
    virtual void drawIndexed(const ARef<VertexArray>& p_varr) = 0;

    static API  getAPI();
    static void setAPI(API p_API);
  private:
    static API s_API;
  };
}


#endif //RENDERER_H
