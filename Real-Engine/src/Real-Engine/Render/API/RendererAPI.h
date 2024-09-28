#ifndef RENDERER_H
#define RENDERER_H


#include "Real-Engine/Core/Core.h"
#include "Real-Engine/Core/Math/math.h"
#include "Real-Engine/Render/API/VertexArray.h"
#include <stdint.h>

namespace Real
{
  class REAL_API RendererAPI
  {
  public:
    enum class API : uint8_t
    {
     NONE = 0, OpenGl = 1
    };
  public:
    virtual ~RendererAPI() = default;
    
    virtual void init() = 0;
    virtual void setViewPort(uint32_t left, uint32_t bottom, uint32_t right, uint32_t top) = 0;
    virtual void clear(const Vec4& p_clear_color) = 0;
    virtual void drawIndexed(const ARef<VertexArray>& p_varr, uint32_t index_count = 0) = 0;
    static API  getAPI();
    static void setAPI(API p_API);
  private:
    static API s_API;
  };
}


#endif //RENDERER_H
