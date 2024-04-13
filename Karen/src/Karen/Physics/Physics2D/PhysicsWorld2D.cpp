#include "Karen/Physics/Physics2D/PhysicsWorld2D.h"
#include "Karen/Physics/Physics2D/Collisions.h"
#include "Karen/Physics/Physics2D/RigidBody.h"


namespace Karen
{
  PhysicsWorld2D::PhysicsWorld2D()
  {

  }

  PhysicsWorld2D::PhysicsWorld2D(size_t its)
  {
    m_bodys.reserve(its);
  }

  void PhysicsWorld2D::onUpdate(Timestep ts)
  {
    for(const auto& b : m_bodys)
    {
      b->setPosition(b->getPosition() + b->getLinearVelocity() * ts.getTime());
    }
    for(uint32_t i = 0; i < m_bodys.size() - 1; ++i)
    {
      auto& b = m_bodys.at(i);
      for(uint32_t j = i; j < m_bodys.size(); ++j)
      {
        collide(b, m_bodys.at(j));
      }
    }
  }
  bool PhysicsWorld2D::collide(const size_t& body1, const size_t& body2)
  {
    return collide(m_bodys.at(body1), m_bodys.at(body2));
  }

  bool PhysicsWorld2D::collide(const ARef<RigidBody2D>& b1, const ARef<RigidBody2D>& b2)
  {
    bool is_collision = false;
    /*const auto& sta2 = Karen::Collision::intersect(r1->getTransformedVertices(), c1->getPosition(), c1->getRadius());
    if(sta2.is_collision)
    {
      KAREN_WARN("collision circle: is: {0}, depth: {1}, normal: {2}", sta2.is_collision, sta2.depth, sta2.normal);
      KAREN_WARN("rect_pos: {0}, circle_pos: {1}", r1->getPosition(), c1->getPosition());
      //r1->setPosition(r1->getPosition() + (sta2.normal * sta2.depth/2.0f));
      //c1->setPosition(c1->getPosition() + (-sta2.normal * sta2.depth/2.0f));
    }*/
    // rect vs rect
    if(b1->m_props.type == ShapeProps::ShapeType::Rectangle && b2->m_props.type == ShapeProps::ShapeType::Rectangle)
    { 
      const auto& sta1 = Karen::Collision::intersect(b1->getTransformedVertices(), b2->getTransformedVertices());
      is_collision = sta1.is_collision;
      if(is_collision)
      {
        KAREN_WARN("collision rect: is: {0}, depth: {1}, normal: {2}", sta1.is_collision, sta1.depth, sta1.normal);
        KAREN_WARN("rect1_pos: {0}, rect2_pos: {1}", b1->getPosition(), b2->getPosition());
        b1->setPosition(b1->getPosition() + (sta1.normal * sta1.depth/2.0f));
        b2->setPosition(b2->getPosition() + (-sta1.normal * sta1.depth/2.0f));
      }
    }

    else if(b1->m_props.type == ShapeProps::ShapeType::Circle && b2->m_props.type == ShapeProps::ShapeType::Circle)
    {
      const auto& sta = Karen::Collision::intersect(b1->getPosition(), b1->getRadius()/2.0f, b2->getPosition(), b2->getRadius()/2.0f);
      is_collision = sta.is_collision;  
      if(is_collision)
      {
        b1->setPosition(b1->getPosition() + (sta.normal * sta.depth/2.0f));
        b2->setPosition(b2->getPosition() + (-sta.normal * sta.depth/2.0f));
      }
    }
    return is_collision;
  }
}

