#include "Entity.h"


namespace Karen
{
  Entity::Entity(entt::entity e, Scene* s)
  {
    m_id = e;
    m_scene = s;
  }
}
