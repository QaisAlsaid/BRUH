#include "Entity.h"
#include "Real-Engine/Scene/Components.h"
#include <sol/forward.hpp>


namespace Real 
{
  Entity::Entity(entt::entity e, Scene* s)
    : m_id(e), m_scene(s)
  {
  }

  Entity::Entity(uint32_t e, Scene* s)
    : m_id((entt::entity)e), m_scene(s)
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

    auto* circ = e.tryGetComponent<CircleComponent>();
    if(circ)
    {
      this->insertComponent<CircleComponent>(*circ);
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

    auto* mc = e.tryGetComponent<MovmentComponent>();
    if(mc)
    {
      this->insertComponent<MovmentComponent>(*mc);
    }

    auto* bcc = e.tryGetComponent<BoxColliderComponent>();
    if(bcc)
    {
      this->insertComponent<BoxColliderComponent>(*bcc);
    }

    auto* sc = e.tryGetComponent<ScriptComponent>();
    if(sc)
    {
      this->insertComponent<ScriptComponent>(*sc);
    }
  }
}

