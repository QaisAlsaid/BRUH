#ifndef RENDER_COMMANDS_H
#define RENDER_COMMANDS_H

#include "Karen/Core/Core.h"
#include "Karen/Core/Math/math.h"
#include "Karen/Render/API/RendererAPI.h"
#include "Karen/Render/API/VertexArray.h"


namespace Karen
{
  class KAREN_API RenderCommands
  {
  public:
    inline static void init()
    {
      s_API->init();
    }
    inline static void setViewPort(uint32_t left, uint32_t bottom, uint32_t right, uint32_t top)
    {
      s_API->setViewPort(left, bottom, right, top);
    }
    inline static void clear(const Vec4& p_clear_color)
    {
      s_API->clear(p_clear_color);
    }
    inline static void drawIndexed(const ARef<VertexArray>& p_varr, uint32_t index_count = 0)
    {
      s_API->drawIndexed(p_varr, index_count);
    }
  private:
    static RendererAPI* s_API;
  };
}

#endif //RENDER_COMMANDS_H
