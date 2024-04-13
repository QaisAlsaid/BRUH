#include "glm/ext/quaternion_geometric.hpp"
#include "pch.h"
#include "Karen/Physics/Physics2D/Collisions.h"
#include <limits>
#include <utility>


namespace Karen
{
  Collision::CollisionStatus Collision::intersect(const Vec2& a_center, float a_radius, const Vec2& b_center, float b_radius)
  {
    CollisionStatus status = {false, 0.0f, Vec2(0.0f, 0.0f)};
    float dist = glm::distance(a_center, b_center);
    float rad = a_radius + b_radius;
    if(dist >= rad)
    {
      return status;
    }
    status.normal = glm::normalize(a_center - b_center);
    status.depth = rad - dist;
    status.is_collision = true;
    return status;
  }

  Collision::CollisionStatus Collision::intersect(const std::vector<Vec2>& a_verts, const std::vector<Vec2>& b_verts)
  {
    CollisionStatus status = {false, std::numeric_limits<float>::max(), Vec2(0.0f, 0.0f)};
    for(size_t i = 0; i < a_verts.size(); ++i)
    {
      auto& v1 = a_verts.at(i);
      auto& v2 = a_verts.at((i+1)%a_verts.size());
      const auto& side = v2 - v1;
      Vec2 axis(-side.y, side.x); // because the loop is clock wise
      const auto& a_min_max = Collision::Project(a_verts, axis);
      const auto& b_min_max = Collision::Project(b_verts, axis);
      const auto& a_min = a_min_max.first; 
      const auto& a_max =  a_min_max.second;
      const auto& b_max = b_min_max.second;
      const auto& b_min = b_min_max.first;
      if(a_min >= b_max || b_min >= a_max)
      {
        status.is_collision = false;
        return status;
      }
      const auto& temp_depth = std::min(b_max - a_min , a_max - b_min);
      if(temp_depth < status.depth)
      {
        status.depth = temp_depth;
        status.normal = axis;
      }
    }

    for(size_t i = 0; i < b_verts.size(); ++i)
    {
      auto& v1 = a_verts.at(i);
      auto& v2 = a_verts.at((i+1)%b_verts.size());
      const auto& side = v2 - v1;
      Vec2 axis(-side.y, side.x); // because the loop is clock wise
      const auto& a_min_max = Collision::Project(a_verts, axis);
      const auto& b_min_max = Collision::Project(b_verts, axis);
      const auto& a_min = a_min_max.first; 
      const auto& a_max =  a_min_max.second;
      const auto& b_max = b_min_max.second;
      const auto& b_min = b_min_max.first;
      if(a_min >= b_max || b_min >= a_max)
      {
        status.is_collision = false;
        return status;
      }
      const auto& temp_depth = std::min(b_max - a_min , a_max - b_min);
      if(temp_depth < status.depth)
      {
        status.depth = temp_depth;
        status.normal = axis;
      }
    }

    status.depth /= glm::length(status.normal);
    status.normal = glm::normalize(status.normal);
    auto ca = getCenter(a_verts); 
    auto cb = getCenter(b_verts);
    const auto& dir = ca - cb;
    if(glm::dot(dir, status.normal) < 0.0f)
    {
      status.normal = -status.normal;
    }
    status.is_collision = true;
    return status;
  }

  Collision::CollisionStatus Collision::intersect(const std::vector<Vec2>& pvts, const Vec2& cen, float rad)
  {
    CollisionStatus status = {false, 0.0f, Vec2(0.0f)};
    for(size_t i = 0; i < pvts.size(); ++i)
    {
      auto& v1 = pvts.at(i);
      auto& v2 = pvts.at((i+1)%pvts.size());
      const auto& side = v2 - v1;
      const Vec2 axis(-side.y, side.x); // because the loop is clock wise
      const auto& a_min_max = Project(pvts, axis);
      const auto& b_min_max = Project(cen, rad, axis);
      const auto& a_min = a_min_max.first; 
      const auto& a_max = a_min_max.second;
      const auto& b_max = b_min_max.second;
      const auto& b_min = b_min_max.first;
      if(a_min >= b_max || b_min >= a_max)
      {
        status.is_collision = false;
        return status;
      }
      const auto& temp_depth = std::min(b_max - a_min , a_max - b_min);
      if(temp_depth < status.depth)
      {
        status.depth = temp_depth;
        status.normal = axis;
      }
    }

    {
      const auto cpidx = getClosestPoint(cen, pvts);
      const auto& cp = pvts.at(cpidx);

      const auto& axis = cp - cen;
      const auto& a_min_max = Collision::Project(pvts, axis);
      const auto& b_min_max = Collision::Project(cen, rad, axis);
      const auto& a_min = a_min_max.first; 
      const auto& a_max = a_min_max.second;
      const auto& b_max = b_min_max.second;
      const auto& b_min = b_min_max.first;
      if(a_min >= b_max || b_min >= a_max)
      {
        status.is_collision = false;
        return status;
      }
      const auto& temp_depth = std::min(b_max - a_min , a_max - b_min);
      if(temp_depth < status.depth)
      {
        status.depth = temp_depth;
        status.normal = axis;
      }

      status.depth /= glm::length(status.normal);
      status.normal = glm::normalize(status.normal);
      auto ca = getCenter(pvts);
      const auto& dir = ca - cen;
      if(glm::dot(dir, status.normal) < 0.0f)
      {
        status.normal = -status.normal;
      }
      status.is_collision = true;
      return status;
    }
  }
  FloatPair Collision::Project(const Vec2& cen, float rad, const Vec2& axis)
  {
    const auto& dir = glm::normalize(axis);
    const auto& dir_rad = dir * rad;
    const auto& p1 = cen + dir_rad;
    const auto& p2 = cen - dir_rad;
    const auto& min = glm::dot(p1, axis);
    const auto& max = glm::dot(p2, axis);
    if(min > max) return std::make_pair(max, min);
    return std::make_pair(min, max);
  }

  FloatPair Collision::Project(const std::vector<Vec2>& verts, const Vec2& axis)
  {
    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();
    for(const auto& vert : verts)
    {
      float proj = glm::dot(vert, axis);
      if(proj < min) min = proj;
      if(proj > max) max = proj;
    }
    return std::make_pair(min, max);
  }

  uint32_t Collision::getClosestPoint(const Vec2& cen, const std::vector<Vec2>& pvts)
  {
    uint32_t idx = 0;
    float min_dist = std::numeric_limits<float>::max();

    for(uint32_t i = 0; i < pvts.size(); ++i)
    {
      const auto& dist = glm::distance(pvts.at(i), cen); 
      if(dist < min_dist)
      {
        min_dist = dist;
        idx = i;
      }
    }
    return idx;
  }

  Vec2 Collision::getCenter(const std::vector<Vec2>&vts)
  {
    Vec2 c(0.0f);
    for(auto& v : vts)
    {
      c += v;
    }
    return c;
  }

}
