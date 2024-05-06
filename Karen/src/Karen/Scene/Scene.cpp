#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Karen/Render/Renderer2D/Renderer2D.h"


namespace Karen
{
  Entity Scene::addEntity(const std::string& tag)
  {
    Entity e(m_registry.create(), this);
    e.addComponent<TransformComponent>();
    if(tag == "")
      e.addComponent<TagComponent>("Entity");
    else 
      e.addComponent<TagComponent>(tag);
    return e;
  }
  
  void Scene::removeEntity(const Entity& e)
  {
    m_registry.destroy(e);
  }

  void Scene::onStart()
  {
    m_registry.view<NativeScriptComponent>().each([&](auto e, auto& native_script)
    {
      native_script.instance = native_script.instantiateScript();
      native_script.instance->m_entity = Entity(e, this);
      native_script.instance->onCreate();
    });
  }

  void Scene::onUpdate(Timestep ts)
  {
    m_registry.view<NativeScriptComponent>().each([&](auto e, auto& native_script)
    {
      native_script.instance->onUpdate(ts);
    });

    SceneCamera* camera = nullptr;
    Mat4 camera_trans;
    {
      auto view = m_registry.view<TransformComponent, CameraComponent>();
      for(auto e : view)
      {
        auto [trans, cam] = view.get<TransformComponent, CameraComponent>(e);
        if(cam.is_primary)
        {
          camera_trans = trans.getTransformationMatrix();
          camera = &cam.camera;
          break;
        }
      }
    }
    if(camera) [[likely]]
    {
      Renderer2D::beginScene(*camera, camera_trans);
      auto group = m_registry.group<TransformComponent>(entt::get<SpriteComponent>);
      for(auto e : group)
      {
        auto [trans, sprite] = group.get<TransformComponent, SpriteComponent>(e);
        Renderer2D::drawQuad(trans.getTransformationMatrix(), sprite.color);
      }
      Renderer2D::endScene();
    }
    else KAREN_CORE_WARN("No main Camera found in Scene");
  }

  void Scene::onViewportResize(uint32_t w, uint32_t h)
  {
    m_viewport_width = w;
    m_viewport_height = h;
    KAREN_WARN("WIDTH {0} HEIGHT {1}", w, h);
    auto view = m_registry.view<CameraComponent>();
    for(auto e : view)
    {
      auto& cam = view.get<CameraComponent>(e);
      if(!cam.is_fixed_aspect_ratio)
        cam.camera.setViewport(m_viewport_width, m_viewport_height);
    }
  }

  void Scene::onEnd()
  {
    m_registry.view<NativeScriptComponent>().each([&](auto e, auto& native_script)
    {
      native_script.instance->onDestroy();
    });
  }
}
