#ifndef RIGID_BODY_2D_H
#define RIGID_BODY_2D_H


#include "Karen/Core/Core.h"
#include "Karen/Core/Math/math.h"


namespace Karen
{
  struct KAREN_API ShapeProps
  {
    enum class ShapeType
    {
      None = 0,
      Rectangle, Circle
    };

    float density;
    float mass;
    float restitution;
    float area;
    bool  is_static;
    ShapeType type;

    ShapeProps(float p_density = 0.0f, float p_mass = 0.0f, float p_rest = 0.0f, float p_area = 0.0f,
        bool p_is_static = true, ShapeType p_type = ShapeType::None)
      : density(p_density), mass(p_mass), restitution(p_rest), area(p_area),
        is_static(p_is_static), type(p_type), radius(0.0f), size(0.0f)
    { 
    };
  private:
    float radius;
    Vec2 size;
    std::vector<Vec2> vertices;
    friend class RigidBody2D;
  };


  class KAREN_API RigidBody2D
  {
  public:
    //TODO: make it a regular Ref
    static ARef<RigidBody2D> createCircle(ShapeProps props, float radius, Vec2 position = Vec2(0.0f), float rotation_angle = 0.0f,
        Vec2 linear_velocity = Vec2(0.0f), float angular_velocity = 0.0f);
    
    static ARef<RigidBody2D> createRectangle(ShapeProps props, Vec2 size, Vec2 position = Vec2(0.0f), float rotation_angle = 0.0f,
        Vec2 linear_velocity = Vec2(0.0f), float angular_velocity = 0.0f);
  
    const Vec2& getSize() const;
    void setSize(const Vec2& size);
    float getRadius() const;
    void setRadius(float radius);
    inline void setLinearVelocity(const Vec2& lvel) {m_linear_velocity = lvel;}
    inline const Vec2& getLinearVelocity() {return m_linear_velocity;}
    inline const Vec2& getPosition() const {return m_position;}
    inline void setPosition(const Vec2& position) {m_position = position;}
    inline float getRotation() const {return m_angle;}
    inline void setRotation(float angle) {m_angle = angle; m_sin = std::sin(m_angle), m_cos = std::cos(m_angle);}
    inline std::vector<Vec2> getVertices()
    {
      KAREN_CORE_ASSERT(m_props.type == ShapeProps::ShapeType::Rectangle, "Can't get vertices from Circle Shape, did you mean to call getRadius()");
      return m_props.vertices;
    }
    inline std::vector<Vec2> getTransformedVertices()
    {
      KAREN_CORE_ASSERT(m_props.type == ShapeProps::ShapeType::Rectangle, "Can't get vertices from Circle Shape, did you mean to call getRadius()");
      transformVertices();
      return m_transformed_vertices;
    }
  public:
    ShapeProps m_props;
  private:
    RigidBody2D(ShapeProps props, float radius, Vec2 position = Vec2(0.0f), 
        float angle = 0.0f, Vec2 linear_velocity = Vec2(0.0f), float angular_velocity = 0.0f);
    RigidBody2D(ShapeProps props, Vec2 size, Vec2 position = Vec2(0.0f),
        float angle = 0.0f, Vec2 linear_velocity = Vec2(0.0f), float angular_velocity = 0.0f);
    void transformVertices();
    void createVertices();
  private:
    Vec2 m_position;
    Vec2 m_linear_velocity;
    std::vector<Vec2> m_transformed_vertices;
    float m_angle, m_sin, m_cos;
    float m_angular_velocity;
    bool  m_need_to_transform = true;
  };
}
#endif //RIGID_BODY_2D_H
