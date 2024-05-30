#ifndef KR_RENDERER2D_H
#define KR_RENDERER2D_H

#include "Karen/Core/Core.h"
#include "Karen/Render/API/RenderCommands.h"
#include "Karen/Render/API/Shader.h"
#include "Karen/Render/API/Texture.h"
#include "Karen/Render/API/VertexArray.h"
#include "Karen/Render/API/VertexBuffer.h"
#include "Karen/Render/OrthographicCamera.h"
#include "Karen/Render/ShaderManager.h"
#include "Karen/Camera.h"

namespace Karen
{
  class KAREN_API Renderer2D
  {
  public:
    struct KAREN_API Stats 
     {
       uint32_t draw_calls = 0;
       uint32_t quad_count = 0;

       inline uint32_t getVertexCount() const
       {
         return quad_count * 4;
       }
       inline uint32_t getIndexCount() const 
       {
         return quad_count * 6;
       }
       inline void reset()
       {
         draw_calls = 0;
         quad_count = 0;
       }
     };
  public:
    static void init(const std::string& shaders_2d_config_path);

    inline static void clear(const Vec4& clear_color = Vec4(1.0f))
    {
      RenderCommands::clear(clear_color);
    }

    inline static void setClearColor(const Vec4& c)
    {
      s_data->clear_color = c;
    }

    inline static void clear()
    {
      RenderCommands::clear(s_data->clear_color);
    }

    static void beginScene(const OrthographicCamera& camera);//TODO:REOMVE AND the ortho 

    static void beginScene(const Camera& camera, const Mat4& transform);
    static void beginScene(const Mat4& projection, const Mat4& view);

    static void drawQuad(const Mat4& transform, const Vec4& color = Vec4(1.0f));
    static void drawQuad(const Mat4& transform, const ARef<Texture2D>& tux, const Vec4& color = Vec4(1.0f));
    
    static void drawQuad(int entity_id, const Mat4& transform, const Vec4& color = Vec4(1.0f));
    static void drawQuad(int entity_id, const Mat4& transform, const ARef<Texture2D>& tux, const Vec4& color = Vec4(1.0f));
    
    static void drawQuad(const Vec3& pos = Vec3(0.0f), const Vec2& size = Vec2(1.0f), float rotation = 0.0f, const Vec4& color = Vec4(1.0f)); 
    static void drawQuad(const Vec2& pos = Vec2(0.0f), const Vec2& size = Vec2(1.0f), float rotation = 0.0f, const Vec4& color = Vec4(1.0f));
     
    static void drawQuad(const Vec3& pos = Vec3(0.0f), const Vec2& size = Vec2(1.0f), float rotation = 0.0f, const ARef<Texture2D>& tux = nullptr, const Vec4& color = Vec4(1.0f));
    static void drawQuad(const Vec2& pos = Vec2(0.0f), const Vec2& size = Vec2(1.0f), float rotation = 0.0f, const ARef<Texture2D>& tux = nullptr, const Vec4& color = Vec4(1.0f));
    
    static void drawQuad(const Vec3& pos = Vec3(0.0f), const Vec2& size = Vec2(1.0f), const Vec4& color = Vec4(1.0f));
    static void drawQuad(const Vec2& pos = Vec2(0.0f), const Vec2& size = Vec2(1.0f), const Vec4& color = Vec4(1.0f));


    static void drawQuad(const Vec3& pos = Vec3(0.0f), const Vec2& size = Vec2(1.0f), const ARef<Texture2D>& tux = nullptr, const Vec4& color = Vec4(1.0f));
    static void drawQuad(const Vec2& pos = Vec2(0.0f), const Vec2& size = Vec2(1.0f), const ARef<Texture2D>& tux = nullptr, const Vec4& color = Vec4(1.0f));
    
    static void flush();
    static void endScene();
  
    static void shutDown();
  private:
    static void reset();
  private:
    struct QuadVertex
     {
       glm::vec4 color;
       glm::vec3 position;
       glm::vec2 tux_coord;
       float     tux_idx;
       bool operator < (const QuadVertex& other)
       {
         return this->position.z - s_data->camera_view[3][3] < other.position.z - s_data->camera_view[3][3]; 
 //        return s_data->camera_view[3][3] - this->position.z < 
       }
       bool operator > (const QuadVertex& other)
       {
         return this->position.z - s_data->camera_view[3][3] > other.position.z - s_data->camera_view[3][3]; 
 //        return s_data->camera_view[3][3] - this->position.z < 
       }

     };
     
     struct Data
     {
       const uint32_t MAX_QUADS = 10000;
       const uint32_t MAX_VERTS = MAX_QUADS * 4;
       const uint32_t MAX_INDES = MAX_QUADS * 6;

       ARef<VertexArray> quad_vertex_arr;
       ARef<Texture2D> wh_tux;
       ARef<VertexBuffer> quad_vertex_buff;
       ShaderManager shaders;
       QuadVertex* quad_vertex_base = nullptr;
       QuadVertex* quad_vertex_ptr  = nullptr;
       uint32_t quad_index_count = 0;
       
       uint32_t MAX_TEXTURE_SLOTS = 8; //<= HACK
       ARef<Texture>* texture_slots;
       uint32_t texture_slot_index = 1;
       Vec4 quad_vertex_pos[4];
       Stats stats;
       Mat4 camera_view = Mat4(1.0f);
       Vec4 clear_color = Vec4(0.2, 0.2, 0.2, 1.0f);
     };

    static Data* s_data;
  public:
    inline static Stats getStats()
    {
      return s_data->stats;
    }

    inline static void resetStats()
    {
      s_data->stats.reset();
    }
  };
}

#endif //KR_RENDERER2D_H
