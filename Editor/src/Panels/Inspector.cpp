#include "Inspector.h"
#include "Karen/Scene/Components.h"
#include "glm/trigonometric.hpp"
#include "imgui.h"



namespace Karen
{
  static const char* projection_type_str[2] = {"Orthographic", "Perspective"};
  static const char* texture_type_str[2] = {"None", "Texture2D"};
  static const char* body_type_str[3] = {"Static", "Kinematic", "Dynamic"};

  template<typename T>
  void drawComponent(Entity& e, const char* label, std::function<void(T*, bool removed)> func)
  {
    T* comp = e.tryGetComponent<T>();
    if(comp)
    {
      ImGui::Text("%s", label);
      ImGui::SameLine();
      bool removed = false;
      if(ImGui::Button((std::string("Remove ") + label).c_str()))
        removed = true;
      ImGui::Separator();
      func(comp, removed);
    }
  }


  void Inspector::onGuiUpdate()
  {
    ImGui::Begin("Inspector");
    
    if(m_current)
    {
      drawComponent<TagComponent>(m_current, "Tag", [](auto* tag_comp, bool removed)
      {
        auto& tag = tag_comp->name;
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, tag.c_str());
        if(ImGui::InputText("##Tag", buffer, sizeof(buffer)))
        {
          tag = std::string(buffer);
        }
      }); 

      drawComponent<ScriptComponent>(m_current, "Script", [](auto* script_comp, bool removed)
      {
        auto& path = script_comp->path;
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, path.c_str());
        if(ImGui::InputText("ScriptPath", buffer, sizeof(buffer)))
        {
          path = std::string(buffer);
        }
      });

      drawComponent<TransformComponent>(m_current, "Transform", [&](auto* trans_comp, bool removed)
      {
        ImGui::DragFloat3("Position", glm::value_ptr(trans_comp->position), 0.25f/((trans_comp->scale.x + trans_comp->scale.y)/2.0f));
        ImGui::DragFloat3("Scale", glm::value_ptr(trans_comp->scale), 0.25f/((trans_comp->scale.x + trans_comp->scale.y)/2.0f));
        Vec3& rad_rot = trans_comp->rotation;
        Vec3 deg_rot = glm::degrees(rad_rot);
        ImGui::DragFloat3("Rotation", glm::value_ptr(deg_rot), 0.25f);
        rad_rot = glm::radians(deg_rot);
        
        if(removed)
          m_current.removeComponent<TransformComponent>();
      });

      drawComponent<CameraComponent>(m_current, "Camera", [&](auto* cam_comp, bool removed)
      {
        bool any_changes = false;
        SceneCamera& cam = cam_comp->camera;
        const char* current_projection_type_str = projection_type_str[(int)cam.getType()];
        ImGui::Checkbox("Main Camera", &cam_comp->is_primary);
        if(ImGui::BeginCombo("Projection Type", current_projection_type_str))
        {
          for(uint8_t i = 0; i < 2; ++i)
          {
            bool is_selected = projection_type_str[i] == current_projection_type_str; //its ok because the ptrs are the same and we are not making string comparision
            if(ImGui::Selectable(projection_type_str[i], is_selected))
            {
              current_projection_type_str = projection_type_str[i];
              cam.setType((SceneCamera::ProjectionType)i);
            }
            if(is_selected) ImGui::SetItemDefaultFocus();
          }
          ImGui::EndCombo();
        }
        switch((int)cam.getType())
        {
          case (int)SceneCamera::ProjectionType::Orthographic:
          {
            auto od = cam.getOrthographicData();
            
            if(ImGui::DragFloat("Size", &od.size)
            || ImGui::DragFloat("Near", &od.near_clip)
            || ImGui::DragFloat("Far", &od.far_clip)) any_changes = true;
            
            if(any_changes)
              cam.setOrthographicData(od);
            
            break;
          }
          case (int)SceneCamera::ProjectionType::Perspective:
          {
            auto pd = cam.getPerspectiveData();
            float deg_fov = glm::degrees(pd.fov);
            if(ImGui::DragFloat("FOV", &deg_fov)
            || ImGui::DragFloat("Near", &pd.near_clip)
            || ImGui::DragFloat("Far", &pd.far_clip)) any_changes = true;
            
            if(any_changes)
            {
              pd.fov = glm::radians(deg_fov);
              cam.setPerspectiveData(pd);
            }
            break;
          }
        }
        if(removed)
          m_current.removeComponent<CameraComponent>();
      });

      drawComponent<SpriteComponent>(m_current, "Sprite", [&](auto* sprite_comp, bool removed)
      {
        const char* current_texture_type_str = texture_type_str[0];
        if(ImGui::BeginCombo("Select Texture", current_texture_type_str))
        {
          for(uint8_t i = 0; i < 2; ++i)
          {
            bool is_selected = texture_type_str[i] == current_texture_type_str;
            if(ImGui::Selectable(texture_type_str[i], is_selected))
            {
              current_texture_type_str = texture_type_str[i];
              //open select from Asset manager modal
            }
            if(is_selected) ImGui::SetItemDefaultFocus();
          }
          ImGui::EndCombo();
        }
        if(ImGui::BeginDragDropTarget())
        {
          if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE2D_ASSET_HANDEL"))
          {
            const char* asset_handel = (const char*)payload->Data;
            sprite_comp->texture_handel = asset_handel;
            current_texture_type_str = texture_type_str[1];
          }
          ImGui::EndDragDropTarget();
        }
        ImGui::Text("TextureHandel %s", sprite_comp->texture_handel.c_str());
        ImGui::ColorEdit4("Color", glm::value_ptr(sprite_comp->color)); 
        if(removed)
        {
          m_current.removeComponent<SpriteComponent>();
        }
      });

      drawComponent<RigidBody2DComponent>(m_current, "Rigid Body", [&](auto* rb2dc, bool removed)
      {
        const char* current_body_type_str = body_type_str[(int)rb2dc->type];
        if(ImGui::BeginCombo("Body Type", current_body_type_str))
        {
          for(uint8_t i = 0; i < 3; ++i)
          {
            bool is_selected = body_type_str[i] == current_body_type_str;
            if(ImGui::Selectable(body_type_str[i], is_selected))
            {
              current_body_type_str = body_type_str[i];
              rb2dc->type = (RigidBody2DComponent::BodyType)i;  
            }
            if(is_selected) ImGui::SetItemDefaultFocus();
          }
          ImGui::EndCombo();
        }
        ImGui::Checkbox("Fixed Rotation", &rb2dc->fixed_rotation);
        if(removed)
          m_current.removeComponent<RigidBody2DComponent>();
      });

      drawComponent<BoxColliderComponent>(m_current, "Box Collider", [&](auto* bcc, bool removed)
      {
        ImGui::DragFloat2("Offset", glm::value_ptr(bcc->offset));
        ImGui::DragFloat2("Size", glm::value_ptr(bcc->size));
        ImGui::DragFloat("Density", &bcc->density);
        ImGui::DragFloat("Friction", &bcc->friction);
        ImGui::DragFloat("Restitution", &bcc->restitution, 0.0f, 1.0f);
        ImGui::SameLine();
        ImGui::DragFloat("Threshold", &bcc->restitution_threshold, 0.0f, 1.0f);
        if(removed)
          m_current.removeComponent<BoxColliderComponent>();
        });
      
      if(ImGui::Button("Add Component"))
        ImGui::OpenPopup("AddComponent");
      if(ImGui::BeginPopupModal("AddComponent"))
      {
        if(ImGui::MenuItem("Transform"))
          m_current.insertComponent<TransformComponent>();
        if(ImGui::MenuItem("Script"))
          m_current.insertComponent<ScriptComponent>();
        //if(ImGui::MenuItem("Tag"))
        //  m_current.addComponent<TagComponent>();
        if(ImGui::MenuItem("Camera"))
          m_current.insertComponent<CameraComponent>();
        if(ImGui::MenuItem("Sprite"))
          m_current.insertComponent<SpriteComponent>();
        if(ImGui::MenuItem("Rigid Body 2D"))
          m_current.insertComponent<RigidBody2DComponent>();
        if(ImGui::MenuItem("Box Collider 2D"))
          m_current.insertComponent<BoxColliderComponent>();
        ImGui::EndPopup();
        ImGui::CloseCurrentPopup();
      }
    }
    
    ImGui::End();
  }
}
