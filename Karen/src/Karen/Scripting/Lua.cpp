#include <pch.h>
#include <sol/forward.hpp>
#include <sol/object.hpp>
#include <sol/overload.hpp>
#include <sol/raii.hpp>
#include <sol/resolve.hpp>
#include <sol/types.hpp>

#include "Lua.h"
#include "Karen/Core/Log.h"
#include "Karen/Core/Core.h"
#include "Karen/Core/Math/math.h"
#include "Karen/Core/UUID.h"
#include "Karen/Core/Window.h"
#include "Karen/Render/Renderer2D/Renderer2D.h"
#include "Karen/Scene/Components.h"
#include "Karen/Scene/Entity.h"
#include "Script.h"
#include "glm/detail/qualifier.hpp"
#include "glm/fwd.hpp"

#define STRINGIFY(x) #x

#define ADD_COMPONENT_FUNCTIONS(component_prefex) \
    auto add_##component_prefex        = &::Karen::Entity::addComponent<component_prefex##Component>; \
    auto insert_##component_prefex     = &::Karen::Entity::insertComponent<component_prefex##Component>; \
    auto get_##component_prefex        = &::Karen::Entity::getComponent<component_prefex##Component>; \
    auto try_get_##component_prefex    = &::Karen::Entity::tryGetComponent<component_prefex##Component>; \
    auto remove_##component_prefex     = &::Karen::Entity::removeComponent<component_prefex##Component>; \
    auto try_remove_##component_prefex = &::Karen::Entity::tryRemoveComponent<component_prefex##Component>; \

#define REGESTER_COMPONENT_FUNCTIONS(component_prefex, usertype_name) \
  {\
    usertype_name[STRINGIFY(add##component_prefex)]       = add_##component_prefex; \
    usertype_name[STRINGIFY(insert##component_prefex)]    = insert_##component_prefex; \
    usertype_name[STRINGIFY(get##component_prefex)]       = get_##component_prefex; \
    usertype_name[STRINGIFY(tryGet##component_prefex)]    = try_get_##component_prefex; \
    usertype_name[STRINGIFY(remove##component_prefex)]    = remove_##component_prefex; \
    usertype_name[STRINGIFY(tryRemove##component_prefex)] = try_remove_##component_prefex; \
  }

namespace Karen
{
  sol::state Lua::lua = sol::state{};
  sol::table Lua::karen = sol::table{};
  
  void Lua::init()
  {
    lua.open_libraries();
    
    karen = Lua::get()["Karen"].get_or_create<sol::table>();
    
    initCore();
    initMath();
    initRenderer();
    initScene();


#if 0
    karen.new_usertype<Scene>("Scene", "addEntity", sol::resolve<Entity(const std::string&)>(&Scene::addEntity), 
        "clear", &Scene::clear, "removeEntity", &Scene::removeEntity);

    karen.new_usertype<TransformComponent>("TransformComponent",
        "position", &TransformComponent::position, "rotation", &TransformComponent::rotation, 
        "scale", &TransformComponent::scale);
    karen.new_usertype<SpriteComponent>("SpriteComponent", 
        "color", &SpriteComponent::color, "handle", &SpriteComponent::texture_handel);
    
/*    karen.new_usertype<RigidBody2DComponent>("RigidBody2DComponent", "fixed_rotation", &RigidBody2DComponent::fixed_rotation, "body_type", &RigidBody2DComponent::type, 
        //Bodyype Enum
        "static",  RigidBody2DComponent::BodyType::Static,
        "dynamic", RigidBody2DComponent::BodyType::Dynamic,
        "kinematic", RigidBody2DComponent::BodyType::Kinematic);
  */  
    auto transform_func = &Entity::insertComponent<TransformComponent>;
    auto sprite_func = &Entity::insertComponent<SpriteComponent>;

    auto get_transform = &Entity::getComponent<TransformComponent>;
    auto get_sprite = &Entity::getComponent<SpriteComponent>;


    karen.new_usertype<Entity>("Entity", "insertTransform", transform_func, "insertSprite", sprite_func, "getTransform", get_transform, "getSprite", get_sprite, "getId", &Entity::operator unsigned int, 
        "getUUID", &Entity::operator unsigned long);
    //karen.set_function<void(const Mat4&, const Vec4&)>("drawQuad", &Renderer2D::drawQuad);

    //karen.set_function("getScene", [&](const char* name){return App::get()->assetManager().getScene(name);});
#endif
    sol::usertype<Script> script = karen.new_usertype<Script>("Script");
    script["onCreate"] = &Script::onCreate;
    script["onUpdate"] = &Script::onUpdate;
    script["onDestroy"] = &Script::onDestroy;
    script["entity"] = &Script::entity;
    script["getTimestep"] = &Script::getTimestep;
/*
    lua.script(R"(
    app = App.get();
    app:print();
    )");


	lua.safe_script(R"(
  script = Script.new()
  function on_create() 
    print("created which idk how");
    end
  script.onCreate = on_create();
  function GetObject()
    return script
  end
)");
  //auto res = lua["script"];
  auto res = lua["GetObject"];
  if(res.valid())
  {
    sol::function fun = res;
    Script s = fun();
    std::cout << "Calling from c++\n";
    //s.onCreate();
    std::cout << "Called from c++\n";
  }
  else 
  {
    sol::error e = res;
    std::cerr << e.what();
  }
      //sol::function fun = m_state["d2.run"];
//fun(20);
*/
  }

  void Lua::initCore()
  {
    //App:
    {
      //usertype
      sol::usertype<App> app = karen.new_usertype<App>("App");
      app["print"] = &App::print;
      app["getWindow"] = &App::getWindow;
      app["getAssetManager"] = sol::resolve<AssetManager&(void)>(&App::assetManager);
      //variables
      karen.add("app", App::get());
    }

    //Timestep:
    {
      //usertype
      sol::usertype<Timestep> ts = karen.new_usertype<Timestep>("Timestep");
      ts["getTime"]               = &Timestep::getTime;
      ts["getTimeAsMilliSeconds"] = &Timestep::getTimeAsMilliSeconds;
    }

    //UUID
    {
      sol::usertype<UUID> uuid = karen.new_usertype<UUID>("UUID", 
          sol::constructors<UUID(), UUID(uint64_t)>()
          );
      uuid["asNumber"] = &UUID::operator unsigned long;
    }

    //AssetManager:
    {
      sol::usertype<AssetManager> am = karen.new_usertype<AssetManager>("AssetManager");
      am["getScene"]     = &AssetManager::getScene; 
      am["getTexture2D"] = &AssetManager::getTexture2D;
    }

    //Window
    {
      sol::usertype<Window> wi = karen.new_usertype<Window>("Window");
      wi["getWidth"]  = &Window::getWidth;
      wi["getHieght"] = &Window::getHeight;
      wi["isVsync"]   = &Window::isVsync;
      wi["setVsync"]  = & Window::setVsync;
    }

  }
  //stpq
  void Lua::initMath()
  {
    sol::table math = karen["Math"].get_or_create<sol::table>();

    //Vec4
    {
      //usertype
      sol::usertype<Vec4> v4 = math.new_usertype<Vec4>("Vec4", 
          sol::constructors<Vec4(), Vec4(float), Vec4(float, float, float, float)>());

      v4["x"] = &Vec4::x; v4["r"] = &Vec4::r; v4["s"] = &Vec4::s;
      v4["y"] = &Vec4::y; v4["g"] = &Vec4::g; v4["t"] = &Vec4::t;
      v4["z"] = &Vec4::z; v4["b"] = &Vec4::b; v4["p"] = &Vec4::p;
      v4["w"] = &Vec4::w; v4["a"] = &Vec4::a; v4["q"] = &Vec4::q;
      
      v4["len"] = &Vec4::length;

      v4["add"] = sol::resolve<Vec4(const Vec4&, const Vec4&)>(&glm::operator+);
      v4["sub"] = sol::resolve<Vec4(const Vec4&, const Vec4&)>(glm::operator-);
      v4["mul"] = sol::overload(
          sol::resolve<Vec4(const Vec4&, const Vec4&)>(glm::operator*), 
          sol::resolve<Vec4(const Vec4&, float)>(glm::operator*)
          );
      v4["dev"] = sol::overload(
          sol::resolve<Vec4(const Vec4&, const Vec4&)>(glm::operator/),
          sol::resolve<Vec4(const Vec4&, float)>(glm::operator/)
          );

    }

    //Vec3
    {
      //usertype
      sol::usertype<Vec3> v3 = math.new_usertype<Vec3>("Vec3", 
          sol::constructors<Vec3(), Vec3(float), Vec3(float, float, float)>());
      
      v3["x"] = &Vec3::x; v3["r"] = &Vec3::r; v3["s"] = &Vec3::s;
      v3["y"] = &Vec3::y; v3["g"] = &Vec3::g; v3["t"] = &Vec3::t;
      v3["z"] = &Vec3::z; v3["b"] = &Vec3::b; v3["p"] = &Vec3::p;
      
      v3["len"] = &Vec3::length;

      v3["add"] = sol::resolve<Vec3(const Vec3&, const Vec3&)>(&glm::operator+);
      v3["sub"] = sol::resolve<Vec3(const Vec3&, const Vec3&)>(glm::operator-);
      v3["mul"] = sol::overload(
          sol::resolve<Vec3(const Vec3&, const Vec3&)>(glm::operator*), 
          sol::resolve<Vec3(const Vec3&, float)>(glm::operator*)
          );
      v3["dev"] = sol::overload(
          sol::resolve<Vec3(const Vec3&, const Vec3&)>(glm::operator/),
          sol::resolve<Vec3(const Vec3&, float)>(glm::operator/)
          );
    }

    //Vec2
    {
      //usertype
      sol::usertype<Vec2> v2 = math.new_usertype<Vec2>("Vec2", 
      sol::constructors<Vec2(), Vec2(float), Vec2(float, float)>());

      v2["x"] = &Vec2::x; v2["r"] = &Vec2::r; v2["s"] = &Vec2::s;
      v2["y"] = &Vec2::y; v2["g"] = &Vec2::g; v2["t"] = &Vec2::t;
      
      v2["len"] = &Vec2::length;

      v2["add"] = sol::resolve<Vec2(const Vec2&, const Vec2&)>(&glm::operator+);
      v2["sub"] = sol::resolve<Vec2(const Vec2&, const Vec2&)>(glm::operator-);
      v2["mul"] = sol::overload(
          sol::resolve<Vec2(const Vec2&, const Vec2&)>(glm::operator*), 
          sol::resolve<Vec2(const Vec2&, float)>(glm::operator*)
          );
      v2["dev"] = sol::overload(
          sol::resolve<Vec2(const Vec2&, const Vec2&)>(glm::operator/),
          sol::resolve<Vec2(const Vec2&, float)>(glm::operator/)
          );
    }

    //Mat4
    {
      sol::usertype<Mat4> m4 = math.new_usertype<Mat4>("Mat4", sol::constructors<Mat4(), Mat4(float)>());

      m4["add"] = sol::resolve<Mat4(const Mat4&, const Mat4&)>(&glm::operator+);
      m4["sub"] = sol::resolve<Mat4(const Mat4&, const Mat4&)>(&glm::operator-);
      m4["mul"] = sol::overload(
          sol::resolve<Mat4(const Mat4&, const Mat4&)>(&glm::operator*),
          sol::resolve<Vec4(const Mat4&, const Vec4&)>(&glm::operator*)
          );
      m4["div"] = sol::overload(
          sol::resolve<Mat4(const Mat4&, const Mat4&)>(&glm::operator/),
          sol::resolve<Vec4(const Mat4&, const Vec4&)>(&glm::operator/)
          );
    
      m4["transpose"] = &glm::transpose<4, 4, glm::f32, glm::defaultp>;
      m4["inverse"]   = &glm::inverse<4, 4, glm::f32, glm::defaultp>;
      m4["translate"] = sol::resolve<Mat4(const Mat4&, const Vec3&)>
        (&glm::translate<glm::f32, glm::defaultp>);
      m4["rotate"]    = sol::resolve<Mat4(const Mat4&, glm::f32, const Vec3&)>
        (&glm::rotate<glm::f32, glm::defaultp>);
      m4["scale"]     = sol::resolve<Mat4(const Mat4&, const Vec3&)>
        (&glm::scale<glm::f32, glm::defaultp>);
      m4["decompose"] = &Karen::decompose;
    }

    //Mat3
    {
      sol::usertype<Mat3> m3 = math.new_usertype<Mat3>("Mat3", sol::constructors<Mat3(), Mat3(float)>());

      m3["add"] = sol::resolve<Mat3(const Mat3&, const Mat3&)>(&glm::operator+);
      m3["sub"] = sol::resolve<Mat3(const Mat3&, const Mat3&)>(&glm::operator-);
      m3["mul"] = sol::overload(
          sol::resolve<Mat3(const Mat3&, const Mat3&)>(&glm::operator*),
          sol::resolve<Vec3(const Mat3&, const Vec3&)>(&glm::operator*)
          );
      m3["div"] = sol::overload(
          sol::resolve<Mat3(const Mat3&, const Mat3&)>(&glm::operator/),
          sol::resolve<Vec3(const Mat3&, const Vec3&)>(&glm::operator/)
          );
    
      m3["transpose"] = &glm::transpose<3, 3, glm::f32, glm::defaultp>;
      m3["inverse"]   = &glm::inverse<3, 3, glm::f32, glm::defaultp>;
    }

    //Mat2
    {
      sol::usertype<Mat2> m2 = math.new_usertype<Mat2>("Mat2", sol::constructors<Mat2(), Mat2(float)>());

      m2["add"] = sol::resolve<Mat2(const Mat2&, const Mat2&)>(&glm::operator+);
      m2["sub"] = sol::resolve<Mat2(const Mat2&, const Mat2&)>(&glm::operator-);
      m2["mul"] = sol::overload(
          sol::resolve<Mat2(const Mat2&, const Mat2&)>(&glm::operator*),
          sol::resolve<Vec2(const Mat2&, const Vec2&)>(&glm::operator*)
          );
      m2["div"] = sol::overload(
          sol::resolve<Mat2(const Mat2&, const Mat2&)>(&glm::operator/),
          sol::resolve<Vec2(const Mat2&, const Vec2&)>(&glm::operator/)
          );
    
      m2["transpose"] = &glm::transpose<2, 2, glm::f32, glm::defaultp>;
      m2["inverse"]   = &glm::inverse<2, 2, glm::f32, glm::defaultp>;
    }
  }

  void Lua::initScene()
  {
    sol::table scene = karen["Scene"].get_or_create<sol::table>();

    //components
    {
      sol::table comps = scene["Components"].get_or_create<sol::table>();
      
      //IDComponent
      {
        sol::usertype<IDComponent> id = comps.new_usertype<IDComponent>("UUID");
        id["getUUID"] = &IDComponent::ID;
      }

      //TagComponent
      //TODO: make it (NameComponent)
      {
        sol::usertype<TagComponent> name = comps.new_usertype<TagComponent>("Name"); 
 //           sol::constructors<TagComponent(TagComponent(), TagComponent(const std::string&))>());
        name["name"] = &TagComponent::name;
      }

      //TransformComponent
      {
        sol::usertype<TransformComponent> trans = comps.new_usertype<TransformComponent>("Transform", 
            sol::constructors<TransformComponent(),
            TransformComponent(const Vec3&, const Vec3&, const Vec3&)>());
        trans["position"] = &TransformComponent::position;
        trans["scale"]    = &TransformComponent::scale;
        trans["rotation"] = &TransformComponent::rotation;
        trans["asMatrix"] = &TransformComponent::getTransformationMatrix;
      }

      //SpriteComponent
      {
        //no constructors because it will change soon (with the asset manager)
        sol::usertype<SpriteComponent> sprite = comps.new_usertype<SpriteComponent>("Sprite");
        sprite["color"] = &SpriteComponent::color;
      }

      //ScriptComponent
      {
        sol::usertype<ScriptComponent> script = comps.new_usertype<ScriptComponent>("Script", 
            sol::constructors<ScriptComponent(), ScriptComponent(const std::string& path)>()
            );
        script["path"] = &ScriptComponent::path;
      }

      //CameraComponent
      {
        sol::usertype<CameraComponent> cam = comps.new_usertype<CameraComponent>("Camera");
        cam["camera"]             = &CameraComponent::camera;
        cam["fixed_aspect_ratio"] = &CameraComponent::is_fixed_aspect_ratio;
        cam["primary"]            = &CameraComponent::is_primary;
      }

      //RigidBody2DComponent
      {
        sol::usertype<RigidBody2DComponent> rb2d = comps.new_usertype<RigidBody2DComponent>("RigidBody2D");
        rb2d["type"]           = &RigidBody2DComponent::type;
        rb2d["fixed_rotation"] = &RigidBody2DComponent::fixed_rotation;
        
      }

      //BoxColliderComponent
      {
        sol::usertype<BoxColliderComponent> bc = comps.new_usertype<BoxColliderComponent>("BoxCollider");
        bc["offset"]                = &BoxColliderComponent::offset;
        bc["size"]                  = &BoxColliderComponent::size;
        bc["density"]               = &BoxColliderComponent::density;
        bc["friction"]              = &BoxColliderComponent::friction;
        bc["restitution"]           = &BoxColliderComponent::restitution;
        bc["restitution_threshold"] = &BoxColliderComponent::restitution_threshold;
      }
    }

    //Scene (the object)
    {
      sol::usertype<Scene> scene_obj = scene.new_usertype<Scene>("Scene");
      scene_obj["addEntity"]    = sol::resolve<Entity(const std::string&)>(&Scene::addEntity); 
      scene_obj["copyEntity"]   = &Scene::copyEntity;
      scene_obj["removeEntity"] = &Scene::removeEntity;
      scene_obj["clear"]        = &Scene::clear;
    }
   
    //Entity
    {
      //Regestring functions for all components i'm sure there is much better way
      ADD_COMPONENT_FUNCTIONS(ID);
      ADD_COMPONENT_FUNCTIONS(Tag);
      ADD_COMPONENT_FUNCTIONS(Transform);
      ADD_COMPONENT_FUNCTIONS(Sprite);
      ADD_COMPONENT_FUNCTIONS(Script);
      ADD_COMPONENT_FUNCTIONS(Camera);
      ADD_COMPONENT_FUNCTIONS(RigidBody2D);
      ADD_COMPONENT_FUNCTIONS(BoxCollider);
      
      sol::usertype<Entity> entity = scene.new_usertype<Entity>("Entity");
      entity["getId"]         = &Entity::operator unsigned int;//Debug

      REGESTER_COMPONENT_FUNCTIONS(ID, entity);
      REGESTER_COMPONENT_FUNCTIONS(Tag, entity);
      REGESTER_COMPONENT_FUNCTIONS(Transform, entity);
      REGESTER_COMPONENT_FUNCTIONS(Sprite, entity);
      REGESTER_COMPONENT_FUNCTIONS(Script, entity);
      REGESTER_COMPONENT_FUNCTIONS(Camera, entity);
      REGESTER_COMPONENT_FUNCTIONS(RigidBody2D, entity);
      REGESTER_COMPONENT_FUNCTIONS(BoxCollider, entity);
    }
  }

  void Lua::initRenderer()
  {
    
    //Renderer2D
    {
      sol::table r2d = karen["Renderer2D"].get_or_create<sol::table>();
      r2d.set_function("beginScene", sol::overload(
            sol::resolve<void(const Mat4&, const Mat4&)>(&Renderer2D::beginScene)
            ));
      r2d.set_function("endScene", &Renderer2D::endScene);
      
      r2d.set_function("setClearColor", &Renderer2D::setClearColor);
      r2d.set_function("clear", sol::overload(
            sol::resolve<void(const Vec4&)>(&Renderer2D::clear),
            sol::resolve<void(void)>(&Renderer2D::clear)
            ));

      r2d.set_function("drawQuad", sol::overload(
            sol::resolve<void(const Mat4&, const Vec4&)>(&Renderer2D::drawQuad),
            sol::resolve<void(const Mat4&, const ARef<Texture2D>&, const Vec4&)>(&Renderer2D::drawQuad),
           //TODO: Camera overload but first make it take view insted of transform

            sol::resolve<void(const Vec3&, const Vec2&, const Vec4&)>(&Renderer2D::drawQuad),
            sol::resolve<void(const Vec2&, const Vec2&, const Vec4&)>(&Renderer2D::drawQuad),
            
            sol::resolve<void(const Vec3&, const Vec2&, float, const Vec4&)>(&Renderer2D::drawQuad),
            sol::resolve<void(const Vec2&, const Vec2&, float, const Vec4&)>(&Renderer2D::drawQuad),
            
            sol::resolve<void(const Vec3&, const Vec2&, float, const ARef<Texture2D>&, const Vec4&)>(&Renderer2D::drawQuad),
            sol::resolve<void(const Vec2&, const Vec2&, float, const ARef<Texture2D>&, const Vec4&)>(&Renderer2D::drawQuad),
            
            sol::resolve<void(const Vec3&, const Vec2&, const ARef<Texture2D>&, const Vec4&)>(&Renderer2D::drawQuad),
            sol::resolve<void(const Vec2&, const Vec2&, const ARef<Texture2D>&, const Vec4&)>(&Renderer2D::drawQuad)
            
            ));
    }
  }
}

