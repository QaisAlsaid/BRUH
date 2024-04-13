#include "pch.h"
#include "Karen/Physics/Physics2D/RigidBody.h"
#include <glm/gtx/rotate_vector.hpp>


namespace Karen
{
  ARef<RigidBody2D> RigidBody2D::createCircle(ShapeProps props, float radius, Vec2 position,
      float rotation_angle, Vec2 linear_velocity, float angular_velocity)
  {
    RigidBody2D* rp = new RigidBody2D(props, radius, position, rotation_angle, linear_velocity, angular_velocity);
    ARef<RigidBody2D> arp;
    arp.reset(rp);
    return arp;
  }

  ARef<RigidBody2D> RigidBody2D::createRectangle(ShapeProps props, Vec2 size, Vec2 position,
      float rotation_angle, Vec2 linear_velocity, float angular_velocity)
  {
    RigidBody2D* rp = new RigidBody2D(props, size, position, rotation_angle, linear_velocity, angular_velocity);
    ARef<RigidBody2D> arp;
    arp.reset(rp);
    return arp;
  }

  RigidBody2D::RigidBody2D(ShapeProps props, float radius, Vec2 position,
      float rotation_angle, Vec2 linear_velocity, float angular_velocity)
    : m_props(props), m_position(position), m_linear_velocity(linear_velocity), m_angular_velocity(angular_velocity)
  {
    setRotation(rotation_angle);
    m_props.radius = radius;
    m_props.type = ShapeProps::ShapeType::Circle; 
  }

  RigidBody2D::RigidBody2D(ShapeProps props, Vec2 size, Vec2 position,
      float rotation_angle, Vec2 linear_velocity, float angular_velocity)
    : m_props(props), m_position(position), m_linear_velocity(linear_velocity), m_angular_velocity(angular_velocity)
  {
    setRotation(rotation_angle);
    m_props.size = size;
    m_props.type = ShapeProps::ShapeType::Rectangle;
    createVertices();
  }

  void RigidBody2D::createVertices()
  {
    const auto& size = m_props.size;
    float left = -size.x/2.0f;
    float right = left + size.x;
    float bottom = -size.y/2.0f;
    float top = bottom + size.y;
    m_props.vertices = 
    {
      Vec2(left, top),
      Vec2(right, top),
      Vec2(right, bottom),
      Vec2(left, bottom)
    };
    m_transformed_vertices = m_props.vertices;
    transformVertices();
  }

  void RigidBody2D::setSize(const Vec2& size)
  {
    KAREN_CORE_ASSERT(m_props.type == ShapeProps::ShapeType::Rectangle, "Can't Assign a size for a Circle Shape Did you mean to Call setRadius()");
    m_props.size = size;
  }

  void RigidBody2D::setRadius(float radius)
  {
    KAREN_CORE_ASSERT(m_props.type == ShapeProps::ShapeType::Circle, "Can't Assign a radius for a Rectangle Shape Did you mean to Call setSize()");
    m_props.radius = radius;
  }

  const Vec2& RigidBody2D::getSize() const
  {
    KAREN_CORE_ASSERT(m_props.type == ShapeProps::ShapeType::Rectangle, "Can't get a size form a Circle Shape Did you mean to Call getRadius()");
    return m_props.size;
  }

  float RigidBody2D::getRadius() const
  {
    KAREN_CORE_ASSERT(m_props.type == ShapeProps::ShapeType::Circle, "Can't get a radius form a Rectangle Shape Did you mean to Call getSize()");
    return m_props.radius;
  }

  void RigidBody2D::transformVertices()
  {
    KAREN_CORE_TRACE("transformVertices called");
    auto& v = m_transformed_vertices;
    const auto& size = m_props.size;
    float left = m_position.x - size.x / 2.0f;
    float right = left + size.x;
    float bottom = m_position.y - size.y / 2.0f;
    float top = bottom + size.y;

    v[0] = Vec2(left, top) + glm::normalize(Vec2(m_cos * v[0].x - m_sin * v[0].y, m_sin * v[0].x + m_cos * v[0].y)); 
    v[1] = Vec2(right, top) + glm::normalize(Vec2(m_cos * v[1].x - m_sin * v[1].y, m_sin * v[1].x + m_cos * v[1].y)); 
    v[2] = Vec2(right, bottom) + glm::normalize(Vec2(m_cos * v[2].x - m_sin * v[2].y, m_sin * v[2].x + m_cos * v[2].y)); 
    v[3] = Vec2(left, bottom) + glm::normalize(Vec2(m_cos * v[3].x - m_sin * v[3].y, m_sin * v[3].x + m_cos * v[3].y)); 
    
    KAREN_CORE_TRACE("transformVertices exited");
  }
}


