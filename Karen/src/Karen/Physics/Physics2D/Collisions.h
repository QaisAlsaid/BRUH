#ifndef COLLISION_H
#define COLLISION_H


#include "Karen/Physics/Physics2D/RigidBody.h"
#include "glm/ext/quaternion_geometric.hpp"

typedef std::pair<float, float> FloatPair;

namespace Karen
{
  class Collision
  {
  public:
    struct CollisionStatus
    {
      bool is_collision = false;
      float depth = 0.0f;
      Vec2 normal = {0.0f, 0.0f};
      CollisionStatus(bool is, float depth, Vec2 normal) 
        : is_collision(is), depth(depth), normal(normal) {}
    };

  public:
    static CollisionStatus intersect(const Vec2& a_center, float a_radius, const Vec2& b_center, float b_radius);
    static CollisionStatus intersect(const std::vector<Vec2>& a_vertices, const std::vector<Vec2>& b_vertices);
    static CollisionStatus intersect(const std::vector<Vec2>& poly_vertices, const Vec2& circle_center, float circle_radius);
  private:
    static Vec2 getCenter(const std::vector<Vec2>& vertices);
    static FloatPair Project(const Vec2& circle_center, float circle_radius, const Vec2& axis);
    static FloatPair Project(const std::vector<Vec2>& vertices, const Vec2& axis);
    static uint32_t getClosestPoint(const Vec2& circle_center, const std::vector<Vec2>& poly_vertices);
  };
}


#endif //COLLISION_H
