#ifndef RENDERER2D_H
#define RENDERER2D_H


#include "Karen/Render/API/RenderCommands.h"
#include "Karen/Render/API/Shader.h"
#include "Karen/Render/API/Texture.h"
#include "Karen/Render/API/VertexArray.h"
#include "Karen/Render/OrthographicCamera.h"
#include "Karen/Render/ShaderManager.h"


namespace Karen
{
  class Renderer2D
  {
  public:
    static void init(const std::string& shaders_2d_config_path);

    inline static void clear(const Vec4& clear_color = Vec4(1.0f))
    {
      RenderCommands::clear(clear_color);
    }

    static void beginScene(const OrthographicCamera& camera);
    
    static void drawQuad(const Vec3& pos = Vec3(0.0f), const Vec2& size = Vec2(1.0f), float rotation = 0.0f, const Vec4& color = Vec4(1.0f)); 
    static void drawQuad(const Vec2& pos = Vec2(0.0f), const Vec2& size = Vec2(1.0f), float rotation = 0.0f, const Vec4& color = Vec4(1.0f));
     
    static void drawTexturedQuad(const Vec3& pos = Vec3(0.0f), const Vec2& size = Vec2(1.0f), float rotation = 0.0f, const ARef<Texture2D>& tux = nullptr, const Vec4& color = Vec4(1.0f));
    static void drawTexturedQuad(const Vec2& pos = Vec2(0.0f), const Vec2& size = Vec2(1.0f), float rotation = 0.0f, const ARef<Texture2D>& tux = nullptr, const Vec4& color = Vec4(1.0f));
     
    static void endScene();
  
     static void deinit();
  private:
     struct Data
     {
       ARef<VertexArray> quad_vertex_arr;
       ShaderManager shaders;
     };

     static Data* s_data;
  };
}

#endif //RENDERER2D_H
