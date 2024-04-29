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

  void Scene::onUpdate(Timestep ts)
  {
    Camera* camera;
    Mat4 camera_trans;
    {
      auto group = m_registry.group<TransformComponent>(entt::get<CameraComponent>);
      for(auto e : group)
      {
        const auto& [trans, cam] = group.get<TransformComponent, CameraComponent>(e);
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
        const auto& [trans, sprite] = group.get<TransformComponent, SpriteComponent>(e);
        Renderer2D::drawQuad(trans.getTransformationMatrix(), sprite.color);
      }
      Renderer2D::endScene();
    }
    else KAREN_CORE_WARN("No default Camera found in Scene");
  }
}
