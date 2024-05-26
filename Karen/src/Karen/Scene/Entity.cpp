#include "Entity.h"
#include "Karen/Scene/Components.h"


namespace Karen
{
  Entity::Entity(entt::entity e, Scene* s)
    : m_id(e), m_scene(s)
  {
  }

  Entity::Entity(Entity& e, entt::entity id, Scene* scene)
    : Entity(id, scene)
  {
    auto* tc = e.tryGetComponent<TagComponent>();
    if(tc)
    {
      this->insertComponent<IDComponent>(UUID());
      this->insertComponent<TagComponent>(*tc);
    }

    auto* transc = e.tryGetComponent<TransformComponent>();
    if(transc)  
    {
      this->insertComponent<TransformComponent>(*transc);
    }

    auto* camc = e.tryGetComponent<CameraComponent>();
    if(camc)
    {
      this->insertComponent<CameraComponent>(*camc);
    }
    
    auto* sprc = e.tryGetComponent<SpriteComponent>();
    if(sprc)
    {
      this->insertComponent<SpriteComponent>(*sprc);
    }

    auto* nsc = e.tryGetComponent<NativeScriptComponent>();
    if(nsc)
    {
      this->insertComponent<NativeScriptComponent>(*nsc);
    }
      
    auto* rb2dc = e.tryGetComponent<RigidBody2DComponent>();
    if(rb2dc)
    {
      this->insertComponent<RigidBody2DComponent>(*rb2dc);
    }
      
    auto* bcc = e.tryGetComponent<BoxColliderComponent>();
    if(bcc)
    {
      this->insertComponent<BoxColliderComponent>(*bcc);
    }
  }
}
