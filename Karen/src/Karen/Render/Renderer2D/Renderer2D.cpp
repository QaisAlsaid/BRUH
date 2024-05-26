#include "pch.h"
#include "Renderer2D.h"
#include "Karen/Core/Log.h"
#include "Karen/Render/API/IndexBuffer.h"
#include "Karen/Render/API/RenderCommands.h"
#include "Karen/Render/API/VertexArray.h"
#include "Karen/Render/API/VertexBuffer.h"
#include "glm/ext/matrix_transform.hpp"
#include <cstdint>

namespace Karen
{
  Renderer2D::Data* Renderer2D::s_data = new  Renderer2D::Data;

  void Renderer2D::init(const std::string& shaders_2d_config_path)
  {
    s_data->shaders.LoadConfig(shaders_2d_config_path);
    uint32_t wh_data = 0xffffffff;
    s_data->wh_tux = Texture2D::create(1, 1, sizeof(wh_data), &wh_data);

    /*const float verts[20] =
    {
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
       0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
       0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
      -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };
*/
    /*const uint32_t inds[6] = 
    {
      0, 1, 2,
      2, 3, 0
    };*/
    s_data->texture_slots = new ARef<Texture>[s_data->MAX_TEXTURE_SLOTS];
    s_data->texture_slots[0] = s_data->wh_tux;

    s_data->quad_vertex_base = new QuadVertex[s_data->MAX_VERTS];
    uint32_t* quad_inds = new uint32_t[s_data->MAX_INDES];
    uint32_t offset = 0;
    for(uint32_t i = 0; i < s_data->MAX_INDES; i += 6)
    {
      quad_inds[i + 0] = offset + 0;
      quad_inds[i + 1] = offset + 1;
      quad_inds[i + 2] = offset + 2;

      quad_inds[i + 3] = offset + 2;
      quad_inds[i + 4] = offset + 3;
      quad_inds[i + 5] = offset + 0;

      offset += 4;
    }

    s_data->quad_vertex_buff = VertexBuffer::create(sizeof(QuadVertex) * s_data->MAX_VERTS);
    ARef<IndexBuffer> ib  = IndexBuffer::create(s_data->MAX_INDES, quad_inds, 5);
    Karen::BufferLayout bl = 
    {
      {"color", Karen::ShaderDataType::Float4},
      {"pos", Karen::ShaderDataType::Float3},
      {"tux_coord", Karen::ShaderDataType::Float2},
      {"tux_idx", Karen::ShaderDataType::Float}
    };
    
    s_data->quad_vertex_buff->setLayout(bl);
    s_data->quad_vertex_arr = VertexArray::create();
    s_data->quad_vertex_arr->setIndexBuffer(ib);
    s_data->quad_vertex_arr->addVertexBuffer(s_data->quad_vertex_buff);

    int samplers[8];
    for(uint8_t i = 0; i < sizeof(samplers)/sizeof(samplers[0]); ++i)
    {
      samplers[i] = i;
    }
    s_data->shaders.get("tux_shader")->bind();
    s_data->shaders.get("tux_shader")->setUniform("u_textures", samplers, sizeof(samplers)/sizeof(samplers[0]));
    
    s_data->quad_vertex_pos[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
    s_data->quad_vertex_pos[1] = { 0.5f, -0.5f, 0.0f, 1.0f};
    s_data->quad_vertex_pos[2] = { 0.5f,  0.5f, 0.0f, 1.0f};
    s_data->quad_vertex_pos[3] = {-0.5f,  0.5f, 0.0f, 1.0f};
    resetStats();
    delete[] quad_inds;
  }

  void Renderer2D::beginScene(const OrthographicCamera& camera)
  {
    s_data->shaders.get("tux_shader")->bind();
    s_data->shaders.get("tux_shader")->setUniform("u_proj_view", camera.getProjView());
    s_data->camera_view = camera.getView();
    reset();
  }

  void Renderer2D::beginScene(const Camera& camera, const Mat4& transform)
  {
    beginScene(camera.getProjection(), glm::inverse(transform));
  }

  void Renderer2D::beginScene(const Mat4& projection, const Mat4& view)
  {
    const auto proj_view = projection * view;
    s_data->shaders.get("tux_shader")->bind();
    s_data->shaders.get("tux_shader")->setUniform("u_proj_view", proj_view);
    s_data->camera_view = view;
    reset();
  }


  void Renderer2D::reset()
  {
    s_data->quad_vertex_ptr = s_data->quad_vertex_base;
    s_data->quad_index_count = 0;
    s_data->texture_slot_index = 1;
  }
    
  void Renderer2D::drawQuad(const Mat4& trans, const Vec4& color)
  {
   if(s_data->quad_index_count >= s_data->MAX_INDES)
    {
      flush();
      reset();
    }

    const Vec2 tux_coords[4] = 
    {
      {0.0f, 0.0f},
      {1.0f, 0.0f},
      {1.0f, 1.0f},
      {0.0f, 1.0f}
    };
    for(uint8_t i = 0; i < sizeof(tux_coords)/sizeof(tux_coords[0]); ++i)
    {
      s_data->quad_vertex_ptr->position = trans * s_data->quad_vertex_pos[i];
      s_data->quad_vertex_ptr->color = color;
      s_data->quad_vertex_ptr->tux_coord = tux_coords[i];
      s_data->quad_vertex_ptr->tux_idx = 0.0f;
      s_data->quad_vertex_ptr++;
    }
    s_data->quad_index_count += 6;

    s_data->stats.quad_count++;

  }

  void Renderer2D::drawQuad(int id, const Mat4& trans, const Vec4& color)
  {

  }

  void Renderer2D::drawQuad(const Mat4& trans, const ARef<Texture2D>& tux, const Vec4& color)
  {
    if(s_data->quad_index_count >= s_data->MAX_INDES || s_data->texture_slot_index >= s_data->MAX_TEXTURE_SLOTS)
    {
      flush();
      reset();
    }

    float c_tux_slot = 0.0f;
    for(uint8_t i = 1; i < s_data->texture_slot_index; ++i)
    {
      if(isEqual(s_data->texture_slots[i], tux))
      {
        c_tux_slot = (float)i;
        break;
      }
    }

    if(c_tux_slot == 0.0f)
    {
      c_tux_slot = (float)s_data->texture_slot_index;
      s_data->texture_slots[s_data->texture_slot_index] = tux;
      s_data->texture_slot_index++;
    }
   
    const Vec2 tux_coords[4] =
    {
      {0.0f, 0.0f},
      {1.0f, 0.0f},
      {1.0f, 1.0f},
      {0.0f, 1.0f}
    };
    
    for(uint8_t i = 0; i < sizeof(tux_coords)/sizeof(tux_coords[0]); ++i)
    {
      s_data->quad_vertex_ptr->position = trans * s_data->quad_vertex_pos[i];
      s_data->quad_vertex_ptr->color = color;
      s_data->quad_vertex_ptr->tux_coord = tux_coords[i];
      s_data->quad_vertex_ptr->tux_idx = c_tux_slot;
      s_data->quad_vertex_ptr++;
    }
    s_data->quad_index_count += 6;

    s_data->stats.quad_count++;

  }

  void Renderer2D::drawQuad(const Vec3& pos, const Vec2& size, float rotation, const Vec4& color)
  {
    //clock wise starting from bottom left 
    //the position is defined from the bottom left of quad
    Mat4 trans = glm::translate(Mat4(1.0f), pos);
    trans = glm::rotate(trans, rotation, {0.0f, 0.0f, 1.0f});
    trans = glm::scale(trans, {size.x, size.y, 1.0f});
  
    drawQuad(trans, color);
  }
  
  void Renderer2D::drawQuad(const Vec2& pos, const Vec2& size, float rotation, const Vec4& color)
  {
    drawQuad(Vec3(pos.x, pos.y, 0.0f), size, rotation, color);
  }

  void Renderer2D::drawQuad(const Vec3& pos, const Vec2& size, float rotation, const ARef<Texture2D>& tux, const Vec4& color)
  {
    Mat4 trans = glm::translate(Mat4(1.0f), pos);
    trans = glm::rotate(trans, rotation, {0.0f, 0.0f, 1.0f});
    trans = glm::scale(trans, {size.x, size.y, 1.0f});

    drawQuad(trans, tux, color);
  }
  
  void Renderer2D::drawQuad(const Vec2& pos, const Vec2& size, float rotation, const ARef<Texture2D>& tux, const Vec4& color)
  {
    drawQuad(Vec3(pos.x, pos.y, 0.0f), size, rotation, tux, color);
  }

  void Renderer2D::drawQuad(const Vec3& pos, const Vec2& size, const Vec4& color)
  {
    if(s_data->quad_index_count >= s_data->MAX_INDES || s_data->texture_slot_index >= s_data->MAX_TEXTURE_SLOTS)
    {
      flush();
      reset();
    }

    s_data->quad_vertex_ptr->position = pos;//Vec3(-size.x/2.0f, -size.y/2.0f, 0.0f);
    s_data->quad_vertex_ptr->color = color;
    s_data->quad_vertex_ptr->tux_coord = {0.0f, 0.0f};
    s_data->quad_vertex_ptr->tux_idx = 0.0f;
    s_data->quad_vertex_ptr++;

    s_data->quad_vertex_ptr->position = pos + Vec3(size.x, 0.0f, 0.0f);//Vec3(size.x/2.0f, -size.y/2.0f, 0.0f);
    s_data->quad_vertex_ptr->color = color;
    s_data->quad_vertex_ptr->tux_coord = {1.0f, 0.0f};
    s_data->quad_vertex_ptr->tux_idx = 0.0f;
    s_data->quad_vertex_ptr++;

    s_data->quad_vertex_ptr->position = pos + Vec3(size.x, size.y, 0.0f);//Vec3(size.x/2.0f, size.y/2.0f, 0.0f);
    s_data->quad_vertex_ptr->color = color;
    s_data->quad_vertex_ptr->tux_coord = {1.0f, 1.0f};
    s_data->quad_vertex_ptr->tux_idx = 0.0f;
    s_data->quad_vertex_ptr++;

    s_data->quad_vertex_ptr->position = pos + Vec3(0.0f, size.y, 0.0f);//Vec3(-size.x/2.0f, size.y/2.0f, 0.0f);
    s_data->quad_vertex_ptr->color = color;
    s_data->quad_vertex_ptr->tux_coord = {0.0f, 1.0f};
    s_data->quad_vertex_ptr->tux_idx = 0.0f;
    s_data->quad_vertex_ptr++;

    s_data->quad_index_count += 6;

    s_data->stats.quad_count++;
  }

  void Renderer2D::drawQuad(const Vec2& pos, const Vec2& size, const Vec4& color)
  {
    drawQuad(Vec3(pos.x, pos.y, 0.0f), size, color);
  }

  void Renderer2D::drawQuad(const Vec3& pos, const Vec2& size, const ARef<Texture>& tux, const Vec4& color)
  {
    if(s_data->quad_index_count >= s_data->MAX_INDES || s_data->texture_slot_index >= s_data->MAX_TEXTURE_SLOTS)
    {
      flush();
      reset();
    }

    float c_tux_slot = 0.0f;
    for(uint8_t i = 1; i < s_data->texture_slot_index; ++i)
    {
      if(isEqual(s_data->texture_slots[i], tux))
      {
        c_tux_slot = (float)i;
        break;
      }
    }

    if(c_tux_slot == 0.0f)
    {
      c_tux_slot = (float)s_data->texture_slot_index;
      s_data->texture_slots[s_data->texture_slot_index] = tux;
      s_data->texture_slot_index++;
    }

    s_data->quad_vertex_ptr->position = pos;//+ Vec3(-size.x/2.0f, -size.y/2.0f, 0.0f);
    s_data->quad_vertex_ptr->color = color;
    s_data->quad_vertex_ptr->tux_coord = {0.0f, 0.0f};
    s_data->quad_vertex_ptr->tux_idx = c_tux_slot;
    s_data->quad_vertex_ptr++;

    s_data->quad_vertex_ptr->position = pos + Vec3(size.x, 0.0f, 0.0f);//Vec3(size.x/2.0f, -size.y/2.0f, 0.0f);
    s_data->quad_vertex_ptr->color = color;
    s_data->quad_vertex_ptr->tux_coord = {1.0f, 0.0f};
    s_data->quad_vertex_ptr->tux_idx = c_tux_slot;
    s_data->quad_vertex_ptr++;

    s_data->quad_vertex_ptr->position = pos + Vec3(size.x, size.y, 0.0f);//Vec3(size.x/2.0f, size.y/2.0f, 0.0f);
    s_data->quad_vertex_ptr->color = color;
    s_data->quad_vertex_ptr->tux_coord = {1.0f, 1.0f};
    s_data->quad_vertex_ptr->tux_idx = c_tux_slot;
    s_data->quad_vertex_ptr++;

    s_data->quad_vertex_ptr->position = pos + Vec3(0.0f, size.y, 0.0f);//Vec3(-size.x/2.0f, size.y/2.0f, 0.0f);
    s_data->quad_vertex_ptr->color = color;
    s_data->quad_vertex_ptr->tux_coord = {0.0f, 1.0f};
    s_data->quad_vertex_ptr->tux_idx = c_tux_slot;
    s_data->quad_vertex_ptr++;

    s_data->quad_index_count += 6;

    s_data->stats.quad_count++;
  }

  void Renderer2D::drawQuad(const Vec2& pos, const Vec2& size, const ARef<Texture>& tux, const Vec4& color)
  {
    drawQuad(Vec3(pos.x, pos.y, 0.0f), size, tux, color);
  }

  void Renderer2D::flush()
  {
    uint32_t data_size = (unsigned char*)s_data->quad_vertex_ptr - (unsigned char*)s_data->quad_vertex_base;
    
    //sort for Z position (for blind)
    std::sort(s_data->quad_vertex_base, s_data->quad_vertex_ptr, [&](const QuadVertex& lhs, const QuadVertex& rhs)
    {
      return lhs.position.z - s_data->camera_view[3][3] < rhs.position.z - s_data->camera_view[3][3]; 
    });
    s_data->quad_vertex_buff->setData(data_size, s_data->quad_vertex_base);

    for(uint8_t i = 0; i < s_data->texture_slot_index; ++i)
    {
      s_data->texture_slots[i]->bind(i);
    }
    RenderCommands::drawIndexed(s_data->quad_vertex_arr, s_data->quad_index_count);
    s_data->stats.draw_calls++;
  }
  
  void Renderer2D::endScene()
  {
    flush();
    KAREN_CORE_INFO("Scene end");
  }

  void Renderer2D::shutDown()
  {
    delete[] s_data->texture_slots;
    delete s_data;
  }

}
