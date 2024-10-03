#include "Real-Engine/Core/Log.h"
#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Real-Engine/Core/App.h"
#include "Real-Engine/Render/Renderer2D/Renderer2D.h"

#include "Real-Engine/Scripting/Script.h"
#include "Real-Engine/Scripting/Lua.h"

#include <algorithm>
#include <box2d/box2d.h>
#include <box2d/b2_world.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <cstdint>
#include <string>


namespace Real
{
  Scene::Scene(const std::string& name)
    : m_name(name)
  {

  }

  Scene::Scene(const Scene& other)
    : m_name(other.m_name), m_viewport_width(other.m_viewport_width), m_viewport_height(other.m_viewport_height)
  {
    m_fast_access.clear();
    m_fast_access.reserve(other.m_fast_access.size());
    m_registry.reset();
    //TODO: template
    //                          old / new
    std::unordered_map<uint64_t, flecs::entity> entt_ids;
    auto& cpyreg = other.m_registry;
    {
      auto query = cpyreg.query<IDComponent>();
      query.each([&](auto e, auto& id) 
      {
        std::string name = e.name().c_str();
        auto en = this->addEntity(name);
        const auto& new_uuid = en.addComponent<IDComponent>(id.ID)->ID;
                                         //id
        m_fast_access[(uint64_t)new_uuid] = { en, this };
        entt_ids[(uint64_t)e] = en;
      });
    }
    {
      auto query = cpyreg.query<TransformComponent>();
      query.each([&](auto e, auto& trans)
      {
        entt_ids[e].set(TransformComponent{trans});
      });
    }

    {
      auto query = cpyreg.query<MPIDComponent>();
      query.each([&](auto e, auto& mp_id)
      {
        entt_ids[e].set(MPIDComponent{mp_id});
      });
    }

    {
      auto query = cpyreg.query<CameraComponent>();
      query.each([&](auto e, auto& cam)
      {
        entt_ids[e].set(CameraComponent{cam});
      });
    }
    {
      auto query = cpyreg.query<SpriteComponent>();
      query.each([&](auto e, auto& sprite)
      {
        entt_ids[e].set(SpriteComponent{sprite});
      });
    }
    {
      auto query = cpyreg.query<CircleComponent>();
      query.each([&](auto e, auto& circle)
      {
        entt_ids[e].set(CircleComponent{circle});
      });
    }
    {
      auto query = cpyreg.query<NativeScriptComponent>();
      query.each([&](auto e, auto& ncs)
      {
        entt_ids[e].set(NativeScriptComponent{ncs});
      });
    }
    {
      auto query = cpyreg.query<RigidBody2DComponent>();
      query.each([&](auto e, auto& rb2d)
      {
        entt_ids[e].set(RigidBody2DComponent{rb2d});
      });
    }
    {
      // is that a thing?
      //auto view = cpyreg.view<MovmentComponent>();
      //for(auto& e : view)
      //{
      //  m_registry.emplace_or_replace<MovmentComponent>(entt_ids.at(e), view.get<MovmentComponent>(e));
      //}
    }
    {
      auto query = cpyreg.query<BoxColliderComponent>();
      query.each([&](auto e, auto& bcc)
      {
        entt_ids[e].set(BoxColliderComponent{bcc});
      });
    }
    {
      auto query = cpyreg.query<CircleColliderComponent>();
      query.each([&](auto e, auto& ccc)
      {
        entt_ids[e].set(CircleColliderComponent{ccc});
      });
    }
    {
      auto query = cpyreg.query<ScriptComponent>();
      query.each([&](auto e, auto& sc)
      {
        entt_ids[e].set(ScriptComponent{sc});
      });
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
    Entity e;
    if(tag.empty())
    {
      auto name = changeName("Entity");
      e = {m_registry.entity(name.c_str()), this};
    }
    else
    {
      if(m_registry.lookup(tag.c_str()))
      {
        auto name = changeName(tag);
        REAL_CORE_WARN("Entity with name: {0} already exists changing name to: {1}", 
          tag, name);
        e = {m_registry.entity(tag.c_str()), this};
      }
      else 
        e = {m_registry.entity(tag.c_str()), this};
    }
    const auto& id = e.addComponent<IDComponent>()->ID;
    e.addComponent<TransformComponent>();
    const auto& mp_id = e.addComponent<MPIDComponent>(giveMPID());
    m_mouse_picking_ids[mp_id->ID] = e;
    m_fast_access[(uint64_t)id] = { e, this };
    return e;
  }
 
  Entity Scene::addEntity(UUID uuid, const std::string& tag)
  {
    auto e = addEntity(tag);
    e.getComponent<IDComponent>()->ID = uuid;
    m_fast_access[(uint64_t)uuid] = { e, this };
    return e;
  }

  Entity Scene::copyEntity(Entity& e)
  {
    auto eid = addEntity(e.m_id.name().c_str());
    Entity en(e, eid, this);
    m_fast_access[(uint64_t)en.getComponent<IDComponent>()->ID] = { en, this };
    return en;
  }

  void Scene::removeEntity(const Entity& e)
  {
    const auto& iter = m_fast_access.find((uint64_t)e.getComponent<IDComponent>()->ID);
    m_fast_access.erase(iter);
    recycleID(m_mouse_picking_ids.at(e.getComponent<MPIDComponent>()->ID));
    e.destroy();
  }

  void Scene::clear()
  {
    m_registry.reset();
    m_fast_access.clear();
    m_recycled_ids.clear();
    m_mouse_picking_ids.clear();
    m_current_idx = 0;
  }

  //for now it loads every thing it should load only changed scripts
  //this should be in the file watcher
  void Scene::onLoad()
  {
    auto& lua = Lua::get();
    m_registry.query<ScriptComponent>().each([&](auto e, ScriptComponent& script)
    {
      const auto& script_ptr = AssetManager::get<AssetManager::ScriptAsset>(script.script_handle);
      if(script_ptr->is_valid)
      {
        REAL_CORE_WARN("yes");
        //lua.safe_script_file(script_ptr->meta.path);
        //auto res = lua["GetObject"];
        //if(res.valid())
        //{
        //  sol::function fun = res;
        //  script_ptr->script = fun();
          //std::cout << "from entt::registry: id: " << uint32_t(e)<<std::endl;
          script_ptr->script->entity = { e, this };
        //}
        //else 
        //{
        //  sol::error e = res;
        //  std::cerr << e.what();
        //}
      }
    });
  }

  void Scene::onStart()
  {
    //TODO: FIXME: remove it
    REAL_CORE_ERROR("Called Scene onLoad");
    onLoad();
    REAL_CORE_ERROR("Exited Scene onLoad");
    m_physics_world = new b2World({0.0f, -9.8f});
    
    //scripts might add physics components
    m_registry.query<NativeScriptComponent>().each([&](auto e, auto& native_script)
    {
      native_script.instance = native_script.instantiateScript();
      native_script.instance->m_entity = Entity(e, this);
      native_script.instance->onCreate();
    });

   // auto& lua = Lua::get();

    REAL_CORE_ERROR("query scripts");
    m_registry.query<ScriptComponent>().each([&](auto e, ScriptComponent& script)
    {
      //lua.safe_script_file(script.path);
      //auto res = lua["GetObject"];
      //if(res.valid())
      //{
      //  sol::function fun = res;
      //  script.script = fun();
      //  std::cout << "from entt::registry: id: " << uint32_t(e)<<std::endl; 
      //  script.script->entity = { e, this };
      //  std::cout << "inside Real-Engine::Entity::m_id: " << (uint32_t)script.script->entity << std::endl;
      //  std::cout << "calling onCreate from c++ << ";
      
    const auto& script_ptr = AssetManager::get<AssetManager::ScriptAsset>(script.script_handle);    
    REAL_CORE_ERROR("Calling onCreate for script path: {0}, validity: {1}", script_ptr->meta.path,script_ptr->is_valid);
    //REAL_CORE_ERROR("Script entity: {0}, actual entity: {1}", (uint64_t)script_ptr->script->entity, (uint64_t)e);
    script_ptr->script->onCreate();
      //   std::cout << " >> " << std::endl;
     // }
     // else 
     // {
       // sol::error e = res;
       // std::cerr << e.what();
     // }
    });

    REAL_CORE_ERROR("Done query for scripts Scene::onStart");

    m_registry.query<TransformComponent, RigidBody2DComponent>().each([&](auto e, TransformComponent& tc, RigidBody2DComponent& rb2dc)
    {
      Entity entity(e, this);
      b2BodyDef def;
      def.position = { tc.position.x, tc.position.y };
      def.type = (b2BodyType)rb2dc.type;
      def.angle = tc.rotation.z;

      auto* body = m_physics_world->CreateBody(&def);
      body->SetFixedRotation(rb2dc.fixed_rotation);
      rb2dc.body = body;
      
      auto* bcc = entity.getComponent<BoxColliderComponent>();
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
      auto* ccc = entity.getComponent<CircleColliderComponent>();
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
    m_registry.query<NativeScriptComponent>().each([&](auto& native_script)
    {
      native_script.instance->onUpdate(ts);
    });

    m_registry.query<ScriptComponent>().each([&](auto e, auto& script)
    {
      const auto& script_ptr = AssetManager::get<AssetManager::ScriptAsset>(script.script_handle);
      script_ptr->script->entity = { e, this };
      script_ptr->script->timestep = ts;
      script_ptr->script->onUpdate();
    });
    
    m_registry.query<MovmentComponent, RigidBody2DComponent>().each([&](MovmentComponent& mc, RigidBody2DComponent& rb2dc)
    {
      auto* body = rb2dc.body;
      body->SetLinearVelocity({ mc.linear_velocity.x, mc.linear_velocity.y });
      body->SetAngularVelocity(mc.angular_velocity);
      //Vec2 position(body->GetPosition().x, body->GetPosition().y);
      //tc.position = { position, tc.position.z };
      //tc.rotation.z = body->GetAngle();
    });

    int vel_iters = 6;
    int pos_iters = 2;
    m_physics_world->Step(ts, vel_iters, pos_iters);
    
    m_registry.query<TransformComponent, RigidBody2DComponent>().each([&](TransformComponent& tc, RigidBody2DComponent& rb2dc)
    {
      auto* body = rb2dc.body;
      body->SetGravityScale(rb2dc.gravity_scale);
      Vec2 position(body->GetPosition().x, body->GetPosition().y);
      tc.position = { position, tc.position.z };
      tc.rotation.z = body->GetAngle();
    });

    SceneCamera* camera = nullptr;
    Mat4 camera_trans;
    {
      m_registry.query<TransformComponent, CameraComponent>().each([&](auto e, auto& trans, auto& cam)
      {
        //auto &&[trans, cam] = get<TransformComponent, CameraComponent>(e);
        if(cam.is_primary)
        {
          if(!cam.is_fixed_aspect_ratio)
            cam.camera.setViewport(m_viewport_width, m_viewport_height);
          cam.camera.onUpdate();
          camera_trans = trans.getTransformationMatrix();
          camera = &cam.camera;
        }
      });
    }
    if(camera) [[likely]]
    {
      Renderer2D::beginScene(*camera, camera_trans);
      
      //Quads
      {
        auto query = m_registry.query<TransformComponent, SpriteComponent, MPIDComponent>();
        query.each([](auto e, auto& trans, auto& sprite, auto& mp_id)
        {
          //auto &&[trans, sprite] = view.get<TransformComponent, SpriteComponent>(e);
          if(sprite.texture_handle)
          {
            REAL_CORE_ERROR("Sprite: {0}", AssetManager::get<AssetManager::Texture2DAsset>(sprite.texture_handle)->is_valid);
            Renderer2D::drawQuad(trans.getTransformationMatrix(),
              AssetManager::get<AssetManager::Texture2DAsset>(sprite.texture_handle)->texture, sprite.color, mp_id.ID);
          }
          else
            Renderer2D::drawQuad(trans.getTransformationMatrix(), sprite.color, e);
        });
      }

      //Circles 
      {
        auto query = m_registry.query<TransformComponent, CircleComponent, MPIDComponent>();
        query.each([](auto e, auto& trans, auto& circle, auto& mp_id)
        {
          //auto &&[trans, circle] = view.get<TransformComponent, CircleComponent>(e);
          Renderer2D::drawCircle(trans.getTransformationMatrix(), circle.thickness, circle.blur, circle.color, mp_id.ID);
        });
      }
      Renderer2D::endScene();
    }
    else REAL_CORE_WARN("No main Camera found in Scene");//TODO: make it use editor camera
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
    else REAL_CORE_WARN("No main Camera found in Scene");
*/
    Renderer2D::beginScene(m_editor_camera.projection, m_editor_camera.view);
   
    //Quads
    {
      m_registry.query<TransformComponent, SpriteComponent, MPIDComponent>().each([](auto e, auto& trans, auto& sprite, auto& mp_id)
      {
        REAL_CORE_WARN("query for SpriteComponent: entity: {0}", e);
        //auto &&[trans, sprite] = view.get<TransformComponent, SpriteComponent>(e);
        if(sprite.texture_handle)
        {
          Renderer2D::drawQuad(trans.getTransformationMatrix(),
          AssetManager::get<AssetManager::Texture2DAsset>(sprite.texture_handle)->texture, sprite.color, mp_id.ID);
        }
        else
        {
          Renderer2D::drawQuad(trans.getTransformationMatrix(), sprite.color, mp_id.ID);
        }
      });
    }

    
    //Circles 
    {
      m_registry.query<TransformComponent, CircleComponent, MPIDComponent>().each([](auto e, auto& trans, auto& circle, auto& mp_id)
      {
        REAL_CORE_WARN("query for CircleComponent: entity: {0}", e);
        //auto &&[trans, circle] = view.get<TransformComponent, CircleComponent>(e);
        Renderer2D::drawCircle(trans.getTransformationMatrix(), circle.thickness, circle.blur, circle.color, mp_id.ID);
      });
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
    //auto view = m_registry.view<TagComponent>();
    //for(const auto& e : view)
    //{
    auto e = m_registry.lookup(n.c_str());
    return e == flecs::entity::null() ? Entity{} : Entity(e, this);
    //    return Entity(e, this);
    
    //return {};
  }
  
  Entity Scene::getEntity(UUID id)
  {
    if(m_fast_access.find((uint64_t)id) != m_fast_access.end())
    {
      const auto& pair = m_fast_access.at((uint64_t)id);
      return { pair.first, pair.second };
    }
    REAL_CORE_WARN("Entity with UUID: {0} Not found", id);
    return {};
  }

  Entity Scene::getMPEntity(uint32_t mp_id)
  {
    if(m_mouse_picking_ids.find(mp_id) != m_mouse_picking_ids.end())
      return {m_mouse_picking_ids.at(mp_id), this};
    else if(mp_id == 0) return {};
    REAL_CORE_ERROR("didn't find Entity with MPID: {0}", mp_id);
    return {};
  }

  void Scene::onEnd()
  {
    m_registry.query<NativeScriptComponent>().each([&](auto e, auto& native_script)
    {
      native_script.instance->onDestroy();
    });
    
    m_registry.query<ScriptComponent>().each([&](auto e, auto& script_comp)
    {
      //script.script.entity = { e, this };
      auto script_asset = AssetManager::get<AssetManager::ScriptAsset>(script_comp.script_handle);
      script_asset->script->onDestroy();
    });

    delete m_physics_world;
    m_physics_world = nullptr;
  }

  std::string Scene::changeName(const std::string& n)
  {
    bool found_name = false;
    std::string name = n;
    uint32_t n_ctr = 0;
    while(!found_name)
    {
      if(m_registry.lookup(name.c_str()) != flecs::entity::null())
      {
        name = n + "(" + std::to_string(n_ctr) + ")";
        n_ctr++;
      }
      else found_name = true;
    }
    return name;
  }

  uint32_t Scene::giveMPID()
  {
    uint32_t id = 0;
    if(m_recycled_ids.empty()) id = ++m_current_idx;
    else 
    {
      id = m_recycled_ids.at(m_recycled_ids.size() - 1);
      m_recycled_ids.pop_back();
    }
    return id;
  }

  void Scene::recycleID(uint32_t id)
  {
    if(std::find(m_recycled_ids.begin(), m_recycled_ids.end(), id) != m_recycled_ids.end()) 
      REAL_CORE_WARN("MPID: {0}, is already in recycle"); 
    else [[likely]]
      m_recycled_ids.push_back(id);
  }
}
