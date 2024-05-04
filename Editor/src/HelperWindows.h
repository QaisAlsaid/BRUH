#ifndef KR_HELPER_WINDOWS_H
#define KR_HELPER_WINDOWS_H

#include <Karen/Karen.h>


namespace Karen
{
  struct Stats
  {
    uint32_t renderer2d_draw_calls_count;
    uint32_t renderer2d_quad_count;
    uint32_t renderer2d_vertex_count;
    uint32_t renderer2d_index_count;
    Timestep mainloop_time_step;
    Stats() = default;
    Stats(const Renderer2D::Stats&s, const Timestep& ts)
    {
      setRenderer2DStats(s);
      mainloop_time_step = ts;
    }
    void setRenderer2DStats(const Renderer2D::Stats& s)
    {
      renderer2d_draw_calls_count = s.draw_calls;
      renderer2d_quad_count = s.quad_count;
      renderer2d_vertex_count = s.getVertexCount();
      renderer2d_index_count = s.getIndexCount();
    }
  };

  class HelperWindow
  {
  public:
    virtual ~HelperWindow() = default;
    virtual void onImGuiUpdate() = 0;
  public:
    bool is_active = false;
  };

  class StatsWindow : public HelperWindow
  {
  public:
    StatsWindow() = default;
    void onImGuiUpdate() override;
  public:
    Stats stats;
  private:
  };
}


#endif //KR_HELPER_WINDOWS_H
