#include "Karen/Render/Renderer2D/Renderer2D.h"
#include "Karen/Core/Log.h"
#include "Karen/Render/API/IndexBuffer.h"
#include "Karen/Render/API/RenderCommands.h"
#include "Karen/Render/API/VertexArray.h"
#include "Karen/Render/API/VertexBuffer.h"
#include "glm/ext/matrix_transform.hpp"


namespace Karen
{
  Renderer2D::Data* Renderer2D::s_data = new Renderer2D::Data;

  void Renderer2D::init(const std::string& shaders_2d_config_path)
  {
    s_data->shaders.LoadConfig(shaders_2d_config_path);

    const float verts[20] =
    {
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
       0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
       0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
      -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };

    const uint32_t inds[6] = 
    {
      0, 1, 2,
      2, 3, 0
    };

    ARef<VertexBuffer> vb = VertexBuffer::create(sizeof(verts), verts, 5);
    ARef<IndexBuffer> ib = IndexBuffer::create(sizeof(inds)/sizeof(inds[0]), inds, 5);
    Karen::BufferLayout bl = 
    {
      {"pos", Karen::ShaderDataType::Float3},
      {"tux_coord", Karen::ShaderDataType::Float2}
    };

    vb->setLayout(bl);
    s_data->quad_vertex_arr = VertexArray::create();
    s_data->quad_vertex_arr->setIndexBuffer(ib);
    s_data->quad_vertex_arr->addVertexBuffer(vb);
  }

  void Renderer2D::beginScene(const OrthographicCamera& camera)
  {
    KAREN_CORE_INFO("beginScene called scene");
    s_data->shaders.get("color_shader")->bind();
    s_data->shaders.get("color_shader")->setUniform("u_proj_view", camera.getProjView());

    s_data->shaders.get("tux_shader")->bind();
    s_data->shaders.get("tux_shader")->setUniform("u_proj_view", camera.getProjView());

    KAREN_CORE_INFO("beginScene exited");
  }
    
  void Renderer2D::drawQuad(const Vec3& pos, const Vec2& size, float rotation, const Vec4& color)
  {
    KAREN_CORE_INFO("draw quad vec3 called None tux");
    Mat4 trans = glm::translate(Mat4(1.0f), pos);
    trans = glm::rotate(trans, glm::radians(rotation), Vec3(0, 0, 1));
    trans = glm::scale(trans, Vec3(size.x, size.y, 1.0f));
    const ARef<Shader>& shader = s_data->shaders.get("color_shader");
    shader->bind();
    shader->setUniform("u_trans", trans);
    shader->setUniform("u_color", color);
    RenderCommands::drawIndexed(s_data->quad_vertex_arr);
    KAREN_CORE_INFO("draw quad vec3 exited None tux");
  }
  
  void Renderer2D::drawQuad(const Vec2& pos, const Vec2& size, float rotation, const Vec4& color)
  {
    KAREN_CORE_INFO("draw quad vec2 called None tux");
    Mat4 trans = glm::translate(Mat4(1.0f), Vec3(pos.x, pos.y, 0.0f));
    trans = glm::rotate(trans, glm::radians(rotation), Vec3(0, 0, 1));
    trans = glm::scale(trans, Vec3(size.x, size.y, 1.0f));
    const ARef<Shader>& shader = s_data->shaders.get("color_shader");
    shader->bind();
    shader->setUniform("u_trans", trans);
    shader->setUniform("u_color", color);
    RenderCommands::drawIndexed(s_data->quad_vertex_arr);
    KAREN_CORE_INFO("draw quad vec2 exited None tux");
  }

  void Renderer2D::drawTexturedQuad(const Vec3& pos, const Vec2& size, float rotation, const ARef<Texture2D>& tux, const Vec4& color)
  {
    KAREN_CORE_INFO("draw quad called vec3 tux");
    Mat4 trans = glm::translate(Mat4(1.0f), pos);
    trans = glm::rotate(trans, glm::radians(rotation), Vec3(0, 0, 1));
    trans = glm::scale(trans, Vec3(size.x, size.y, 1.0f));
    const ARef<Shader>& shader = s_data->shaders.get("tux_shader");
    tux->bind(0);
    shader->bind();
    shader->setUniform("u_trans", trans);
    shader->setUniform("u_tint_color", color);
    shader->setUniform("u_tux", 0);
    RenderCommands::drawIndexed(s_data->quad_vertex_arr);
  }
  
  void Renderer2D::drawTexturedQuad(const Vec2& pos, const Vec2& size, float rotation, const ARef<Texture2D>& tux, const Vec4& color)
  {
    KAREN_CORE_INFO("draw quad vec2 called tux");
    Mat4 trans = glm::translate(Mat4(1.0f), Vec3(pos.x, pos.y, 0.0f));
    trans = glm::rotate(trans, glm::radians(rotation), Vec3(0, 0, 1));
    trans = glm::scale(trans, Vec3(size.x, size.y, 1.0f));
    const ARef<Shader>& shader = s_data->shaders.get("tux_shader"); 
    tux->bind(0);
    shader->bind();
    shader->setUniform("u_trans", trans);
    shader->setUniform("u_tint_color", color);
    shader->setUniform("u_tux", 0);
    RenderCommands::drawIndexed(s_data->quad_vertex_arr);
    KAREN_CORE_INFO("draw quad vec2 exited tux");
  }
  
  void Renderer2D::endScene()
  {
    KAREN_CORE_INFO("Scene end");
  }

  void Renderer2D::deinit()
  {
    delete s_data;
  }
}
