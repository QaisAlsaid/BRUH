#ifndef RENDER_COMMANDS_H
#define RENDER_COMMANDS_H

#include "Karen/Core.h"
#include "Karen/Math/math.h"
#include "Render/API/RendererAPI.h"
#include "Render/API/VertexArray.h"


namespace Karen
{
  class KAREN_API RenderCommands
  {
  public:
    static void clear(const Vec4& p_clear_color);
    static void drawIndexed(const ARef<VertexArray>& p_varr);
  private:
    static RendererAPI* s_API;
  };
}

#endif //RENDER_COMMANDS_H
