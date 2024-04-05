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
    static void clear(const Vec4& p_clear_color);
    static void drawIndexed(const ARef<VertexArray>& p_varr);
  private:
    static RendererAPI* s_API;
  };
}

#endif //RENDER_COMMANDS_H
