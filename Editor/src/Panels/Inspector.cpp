#include "Inspector.h"
#include "Karen/Scene/Components.h"
#include "imgui.h"



namespace Karen
{ 
  template<typename T>
  void drawComponent(Entity& e, const char* label, std::function<void(T*, bool removed)> func)
  {
    T* comp = e.tryGetComponent<T>();
    if(comp)
    {
      ImGui::Text(label);
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
      
      drawComponent<TransformComponent>(m_current, "Transform", [&](auto* trans_comp, bool removed)
      {
        ImGui::DragFloat3("Position", glm::value_ptr(trans_comp->position), 0.25f/((trans_comp->scale.x + trans_comp->scale.y)/2.0f));
        ImGui::DragFloat3("Scale", glm::value_ptr(trans_comp->scale), 0.25f/((trans_comp->scale.x + trans_comp->scale.y)/2.0f));
        ImGui::DragFloat3("Rotation", glm::value_ptr(trans_comp->rotation), 0.25f);
        
        if(removed)
          m_current.removeComponent<TransformComponent>();
      });

      drawComponent<CameraComponent>(m_current, "Camera", [&](auto* cam_comp, bool removed)
      {
        bool any_changes = false;
        SceneCamera& cam = cam_comp->camera;
        const char* projection_type_str[2] = {"Orthographic", "Perspective"};
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
            if(ImGui::DragFloat("FOV", &pd.fov)
            || ImGui::DragFloat("Near", &pd.near_clip)
            || ImGui::DragFloat("Far", &pd.far_clip)) any_changes = true;
            
            if(any_changes)
              cam.setPerspectiveData(pd);
            break;
          }
        }
        if(removed)
          m_current.removeComponent<CameraComponent>();
      });

      drawComponent<SpriteComponent>(m_current, "Sprite", [&](auto* sprite_comp, bool removed)
      {
        if(ImGui::Button("Set Texture"))
          sprite_comp->texture_handel = "tux"; //super temp
        ImGui::Text("TextureHandel %s", sprite_comp->texture_handel.c_str());
        ImGui::ColorEdit4("Color", glm::value_ptr(sprite_comp->color)); 
        if(removed)
        {
          m_current.removeComponent<SpriteComponent>();
        }
      });
      
      if(ImGui::Button("Add Component"))
        ImGui::OpenPopup("AddComponent");
      if(ImGui::BeginPopup("AddComponent"))
      {
        if(ImGui::MenuItem("Transform"))
          m_current.insertComponent<TransformComponent>();
        //if(ImGui::MenuItem("Tag"))
        //  m_current.addComponent<TagComponent>();
        if(ImGui::MenuItem("Camera"))
          m_current.insertComponent<CameraComponent>();
        if(ImGui::MenuItem("Sprite"))
          m_current.insertComponent<SpriteComponent>();
        ImGui::EndPopup();
        ImGui::CloseCurrentPopup();
      }
    }
    
    ImGui::End();
  }
}
