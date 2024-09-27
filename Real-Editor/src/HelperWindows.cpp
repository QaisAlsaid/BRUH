//WTF is this

#include "HelperWindows.h"
#include <imgui.h>

namespace Real
{ 
  void StatsWindow::onImGuiUpdate()
  {
    if(is_active)
    {
      ImGui::Begin("Stats");
      ImGui::Text("MainLoop: Timestep: %f", stats.mainloop_time_step.getTime());
      ImGui::Separator();
      ImGui::Text("Renderer2D: Draw Calls: %d", stats.renderer2d_draw_calls_count);
      ImGui::Text("Renderer2D: Quad Count: %d", stats.renderer2d_quad_count);
      ImGui::Text("Renderer2D: Vertex Count: %d", stats.renderer2d_vertex_count);
      ImGui::Text("Renderer2D: Index Count: %d", stats.renderer2d_index_count);
      ImGui::End();
    }
  }
}
