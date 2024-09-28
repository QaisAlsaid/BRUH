#ifndef KR_RENDERER2D_H
#define KR_RENDERER2D_H

#include "Real-Engine/Core/Core.h"
#include "Real-Engine/Render/API/RenderCommands.h"
#include "Real-Engine/Render/API/Shader.h"
#include "Real-Engine/Render/API/Texture.h"
#include "Real-Engine/Render/API/VertexArray.h"
#include "Real-Engine/Render/API/VertexBuffer.h"
#include "Real-Engine/Render/OrthographicCamera.h"
#include "Real-Engine/Render/ShaderManager.h"
#include "Real-Engine/Render/Camera.h"

namespace Real
{
  class REAL_API Renderer2D
  {
  public:
    struct REAL_API Stats 
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

    static void beginScene(const Camera& camera, const Mat4& transform);
    static void beginScene(const Mat4& projection, const Mat4& view);

    //Circle
    static void drawCircle(const Mat4& transform, float thickness, float blur, const Vec4& color = Vec4(1.0f));
    
    //Quad
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
    
    static void endScene();
  
    static void shutDown();
  private:
    static void reset();
    static void flushQuad();
    static void flushCircle();
  private:
    struct QuadVertex
    {
      glm::vec4 color;
      glm::vec3 position;
      glm::vec2 tux_coord;
      float     tux_idx;
    };

    struct CircleVertex
    {
      glm::vec4 color;
      glm::vec3 position;
      glm::vec2 local_position;
      float thickness = 0.1f;
      float blur = 0.005f;
    };

    struct Quad 
    {
      QuadVertex vertices[4];
    };
    
    struct Circle 
    {
      CircleVertex vertices[4];
    };

    struct Data
    {
      const uint32_t MAX_QUADS = 10000;
      const uint32_t MAX_VERTS = MAX_QUADS * 4;
      const uint32_t MAX_INDES = MAX_QUADS * 6;

      ShaderManager shaders;
      
      ARef<Texture2D> wh_tux;
      
      Vec4 quad_vertex_pos[4];
      
      ARef<VertexArray> quad_vertex_arr;
      ARef<VertexBuffer> quad_vertex_buff;
      Quad* quad_base = nullptr;
      Quad* quad_ptr  = nullptr;
      uint32_t quad_index_count = 0;
      
      ARef<VertexArray> circle_vertex_array;
      ARef<VertexBuffer> circle_vertex_buff;
      Circle* circle_base = nullptr;
      Circle* circle_ptr  = nullptr; 
      uint32_t circle_index_count;

      uint32_t MAX_TEXTURE_SLOTS = 8; //<= HACK
      ARef<Texture>* texture_slots;
      uint32_t texture_slot_index = 1;
      Stats stats;
      Mat4 proj_view = Mat4(1.0f);
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
