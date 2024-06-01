#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Karen/Core/App.h"
#include "Karen/Render/Renderer2D/Renderer2D.h"

#include "Karen/Scripting/Script.h"
#include "Karen/Scripting/Lua.h"

#include <box2d/box2d.h>
#include <box2d/b2_world.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <functional>


namespace Karen
{
  Scene::Scene(const std::string& name)
    : m_name(name)
  {

  }

  Scene::Scene(const Scene& other)
    : m_name(other.m_name), m_viewport_width(other.m_viewport_width), m_viewport_height(other.m_viewport_height)
  {
    //TODO: template
    //old / new
    std::unordered_map<entt::entity, entt::entity> entt_ids;
    auto& cpyreg = other.m_registry;
    {
      auto view = cpyreg.view<IDComponent>();
      for(auto& e : view)
      {
        auto en = this->addEntity();
        en.insertComponent<IDComponent>(view.get<IDComponent>(e));
        en.insertComponent<TagComponent>(cpyreg.get<TagComponent>(e));
        entt_ids[e] = en;
      }
    }
    {
      auto view = cpyreg.view<TransformComponent>();
      for(auto& e : view)
      {
        m_registry.emplace_or_replace<TransformComponent>(entt_ids.at(e), view.get<TransformComponent>(e));
      }
    }
    {
      auto view = cpyreg.view<CameraComponent>();
      for(auto& e : view)
      {
        m_registry.emplace_or_replace<CameraComponent>(entt_ids.at(e), view.get<CameraComponent>(e));
      }
    }
    {
      auto view = cpyreg.view<SpriteComponent>();
      for(auto& e : view)
      {
        m_registry.emplace_or_replace<SpriteComponent>(entt_ids.at(e), view.get<SpriteComponent>(e));
      }
    }
    {
      auto view = cpyreg.view<NativeScriptComponent>();
      for(auto& e : view)
      {
        m_registry.emplace_or_replace<NativeScriptComponent>(entt_ids.at(e), view.get<NativeScriptComponent>(e));
      }
    }
    {
      auto view = cpyreg.view<RigidBody2DComponent>();
      for(auto& e : view)
      {
        m_registry.emplace_or_replace<RigidBody2DComponent>(entt_ids.at(e), view.get<RigidBody2DComponent>(e));
      }
    }
    {
      auto view = cpyreg.view<BoxColliderComponent>();
      for(auto& e : view)
      {
        m_registry.emplace_or_replace<BoxColliderComponent>(entt_ids.at(e), view.get<BoxColliderComponent>(e));
      }
    }
    {
      auto view = cpyreg.view<CircleColliderComponent>();
      for(auto& e : view)
      {
        m_registry.emplace_or_replace<CircleColliderComponent>(entt_ids.at(e), view.get<CircleColliderComponent>(e));
      }
    }
    {
      auto view = cpyreg.view<ScriptComponent>();
      for(auto& e : view)
      {
        m_registry.emplace_or_replace<ScriptComponent>(entt_ids.at(e), view.get<ScriptComponent>(e));
      }
    }
  }

  ARef<Scene> Scene::copy(const ARef<Scene>& other)
  {
    Scene* s = new Scene(*other.get());
    auto scene = createARef<Scene>();
    scene.reset(s);
    return scene;
  }

  Entity Scene::addEntity(const std::string& tag)
  {
    Entity e(m_registry.create(), this);
    e.addComponent<IDComponent>();
    e.addComponent<TransformComponent>();
    if(tag == "")
      e.addComponent<TagComponent>("Entity");
    else 
      e.addComponent<TagComponent>(tag);
    return e;
  }
 
  Entity Scene::addEntity(UUID uuid, const std::string& tag)
  {
    auto e = addEntity(tag);
    e.getComponent<IDComponent>().ID = uuid;
    return e;
  }

  Entity Scene::copyEntity(Entity& e)
  {
    auto eid = m_registry.create();
    Entity en(e, eid, this);
    return en;
  }

  void Scene::removeEntity(const Entity& e)
  {
    m_registry.destroy(e);
  }

  void Scene::clear()
  {
    m_registry.clear();
  }

  void Scene::onStart()
  {
    m_physics_world = new b2World({0.0f, -9.8f});
    
    //scripts might add physics components
    m_registry.view<NativeScriptComponent>().each([&](auto e, auto& native_script)
    {
      native_script.instance = native_script.instantiateScript();
      native_script.instance->m_entity = Entity(e, this);
      native_script.instance->onCreate();
    });

    auto& lua = Lua::get();
    m_registry.view<ScriptComponent>().each([&](auto e, ScriptComponent& script)
    {
      lua.safe_script_file(script.path);
      auto res = lua["GetObject"];
      if(res.valid())
      {
        sol::function fun = res;
        script.script = fun();
        std::cout << "from entt::registry: id: " << uint32_t(e)<<std::endl; 
        script.script->entity = { e, this };
        std::cout << "inside Karen::Entity::m_id: " << (uint32_t)script.script->entity << std::endl;
        std::cout << "calling onCreate from c++ << ";
        script.script->onCreate();
        std::cout << " >> " << std::endl;
      }
      else 
      {
        sol::error e = res;
        std::cerr << e.what();
      }
    });


    m_registry.view<TransformComponent, RigidBody2DComponent>().each([&](auto e, TransformComponent& tc, RigidBody2DComponent& rb2dc)
    {
      Entity entity(e, this);
      b2BodyDef def;
      def.position = { tc.position.x, tc.position.y };
      def.type = (b2BodyType)rb2dc.type;
      def.angle = tc.rotation.z;

      auto* body = m_physics_world->CreateBody(&def);
      body->SetFixedRotation(rb2dc.fixed_rotation);
      rb2dc.body = body;
      
      auto* bcc = entity.tryGetComponent<BoxColliderComponent>();
      if(bcc)
      {
        b2PolygonShape poly;
        poly.SetAsBox(bcc->size.y * tc.scale.x, bcc->size.x * tc.scale.y);
        
        b2FixtureDef fixture_def;
        fixture_def.shape = &poly;
        fixture_def.density = bcc->density;
        fixture_def.friction = bcc->friction;
        fixture_def.restitution = bcc->restitution;
        fixture_def.restitutionThreshold = bcc->restitution_threshold;
        /*auto * fixture = */body->CreateFixture(&fixture_def);
        //bcc.fixture = fixture;
      }
      auto* ccc = entity.tryGetComponent<CircleColliderComponent>();
      if(ccc)
      {
        b2CircleShape circle;
        circle.m_radius = ccc->radius;
      
        b2FixtureDef fixture_def;
        fixture_def.shape = &circle;
        fixture_def.density = ccc->density;
        fixture_def.friction = ccc->friction;
        fixture_def.restitution = ccc->restitution;
        fixture_def.restitutionThreshold = ccc->restitution_threshold;
        body->CreateFixture(&fixture_def);
      }
    });
  }

  void Scene::onUpdate(Timestep ts)
  {
    m_registry.view<NativeScriptComponent>().each([&](auto e, auto& native_script)
    {
      native_script.instance->onUpdate(ts);
    });

    m_registry.view<ScriptComponent>().each([&](auto e, auto& script)
    {
      script.script->entity = { e, this };
      script.script->timestep = ts;
      script.script->onUpdate();
    });
    
    int vel_iters = 6;
    int pos_iters = 2;
    m_physics_world->Step(ts, vel_iters, pos_iters);
    
    m_registry.view<TransformComponent, RigidBody2DComponent>().each([&](auto e, TransformComponent& tc, RigidBody2DComponent& rb2dc)
    {
      auto* body = rb2dc.body;
      body->SetGravityScale(rb2dc.gravity_scale);
      body->SetLinearVelocity({ rb2dc.linear_velocity.x, rb2dc.linear_velocity.y });
      body->SetAngularVelocity(rb2dc.angular_velocity);
      Vec2 position(body->GetPosition().x, body->GetPosition().y);
      tc.position = { position, tc.position.z };
      tc.rotation.z = body->GetAngle();
    });

    SceneCamera* camera = nullptr;
    Mat4 camera_trans;
    {
      auto view = m_registry.view<TransformComponent, CameraComponent>();
      for(const auto& e : view)
      {
        auto &&[trans, cam] = view.get<TransformComponent, CameraComponent>(e);
        if(cam.is_primary)
        {
          if(!cam.is_fixed_aspect_ratio)
            cam.camera.setViewport(m_viewport_width, m_viewport_height);
          cam.camera.onUpdate();
          camera_trans = trans.getTransformationMatrix();
          camera = &cam.camera;
          break;
        }
      }
    }
    if(camera) [[likely]]
    {
      Renderer2D::beginScene(*camera, camera_trans);
      auto view = m_registry.view<TransformComponent, SpriteComponent>();
      for(const auto& e : view)
      {
        auto &&[trans, sprite] = view.get<TransformComponent, SpriteComponent>(e);
        if(!sprite.texture_handel.empty())
        {
          Renderer2D::drawQuad(trans.getTransformationMatrix(),
              App::get()->assetManager()->getTexture2D(sprite.texture_handel), sprite.color);
        }
        else
          Renderer2D::drawQuad(trans.getTransformationMatrix(), sprite.color);
      }
      Renderer2D::endScene();
    }
    else KAREN_CORE_WARN("No main Camera found in Scene");//TODO: make it use editor camera
  }

  void Scene::onEditorUpdate(Timestep ts)
  {
    /*SceneCamera* camera = nullptr;
    Mat4 camera_trans;
    {
      auto view = m_registry.view<TransformComponent, CameraComponent>();
      for(const auto& e : view)
      {
        auto &&[trans, cam] = view.get<TransformComponent, CameraComponent>(e);
        if(cam.is_primary)
        {
          if(!cam.is_fixed_aspect_ratio)
            cam.camera.setViewport(m_viewport_width, m_viewport_height);
          cam.camera.onUpdate();
          camera_trans = trans.getTransformationMatrix();
          camera = &cam.camera;
          break;
        }
      }
    }
    if(camera) [[likely]]
    {
      Renderer2D::beginScene(*camera, camera_trans);
      auto view = m_registry.view<TransformComponent, SpriteComponent>();
      for(const auto& e : view)
      {
        auto &&[trans, sprite] = view.get<TransformComponent, SpriteComponent>(e);
        if(!sprite.texture_handel.empty())
        {
          Renderer2D::drawQuad(trans.getTransformationMatrix(),
              App::get()->assetManager().getTexture2D(sprite.texture_handel), sprite.color);
        }
        else
          Renderer2D::drawQuad(trans.getTransformationMatrix(), sprite.color);
      }
      Renderer2D::endScene();
    }
    else KAREN_CORE_WARN("No main Camera found in Scene");
*/
    Renderer2D::beginScene(m_editor_camera.projection, m_editor_camera.view);
    auto view = m_registry.view<TransformComponent, SpriteComponent>();
    for(const auto& e : view)
    {
      auto &&[trans, sprite] = view.get<TransformComponent, SpriteComponent>(e);
      if(!sprite.texture_handel.empty())
      {
        Renderer2D::drawQuad(trans.getTransformationMatrix(),
        App::get()->assetManager()->getTexture2D(sprite.texture_handel), sprite.color);
      }
      else
        Renderer2D::drawQuad(trans.getTransformationMatrix(), sprite.color);
    }
    Renderer2D::endScene();
}

  void Scene::onViewportResize(uint32_t w, uint32_t h)
  {
    m_viewport_width = w;
    m_viewport_height = h;
  }
  
  Entity Scene::getEntity(const std::string& n)
  {
    auto view = m_registry.view<TagComponent>();
    for(const auto& e : view)
    {
      if(view.get<TagComponent>(e).name == n)
        return Entity(e, this);
    }
    return {};
  }
  
  Entity Scene::getEntity(UUID id)
  {
    auto view = m_registry.view<IDComponent>();
    for(const auto& e : view)
    {
      if(view.get<IDComponent>(e).ID == id)
        return Entity(e, this);
    }
    return {};

  }

  void Scene::onEnd()
  {
    m_registry.view<NativeScriptComponent>().each([&](auto e, auto& native_script)
    {
      native_script.instance->onDestroy();
    });
    
    m_registry.view<ScriptComponent>().each([&](auto e, auto& script)
    {
      //script.script.entity = { e, this };
      script.script->onDestroy();
    });

    delete m_physics_world;
    m_physics_world = nullptr;
  }
}
