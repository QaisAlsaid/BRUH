#include "SceneHierarchy.h"
#include "imgui.h"


namespace Karen
{
  SceneHierarchy::SceneHierarchy(const ARef<Scene>& context)
  {
    setContext(context);
  }

  void SceneHierarchy::setContext(const ARef<Scene>& context)
  {
    m_context = context;
  }

  void SceneHierarchy::onGuiUpdate()
  {
    ImGui::Begin("Scene Hierarchy");
    ImGui::PushItemWidth(-1);
    if(ImGui::Button("+"))
    {
      ImGui::OpenPopup("SceneContextWindow");
    }
    ImGui::PopItemWidth();
    m_context->m_registry.view<TagComponent>().each([&](auto e, auto& t)
    {
      Entity en(e, m_context.get());
      drawEntityNode(en);
    });
    
    if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
    {
      m_current = {};
    }
    auto flags = ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight;
    if(ImGui::BeginPopupContextWindow("SceneContextWindow", flags))
    {
      if(ImGui::MenuItem("Add Empty Entity"))
        m_context->addEntity();
      ImGui::EndPopup();
    }
    ImGui::End();
  }

  void SceneHierarchy::drawEntityNode(Entity& e)
  {
    const auto& tag = e.getComponent<TagComponent>().name;
    ImGuiTreeNodeFlags flags = (m_current == e ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    bool opend = ImGui::TreeNodeEx((void*)(uintptr_t)e, flags, "%s",tag.c_str());
    if(ImGui::IsItemClicked())
    {
      m_current = e;
    }
    bool deleted = false;
    auto popup_flags = ImGuiPopupFlags_NoOpenOverExistingPopup | ImGuiPopupFlags_MouseButtonRight;
    if(ImGui::BeginPopupContextItem(std::string("Delete" + std::to_string((uint32_t)e)).c_str(), popup_flags))
    {
      if(ImGui::MenuItem("Delete Entity")) 
        deleted = true;
      ImGui::EndPopup();
    }
    if(opend)
    {
      //recurse for childs 
      ImGui::TreePop();
    }
    if(deleted)
    {
      e.destroy();
      if(m_current == e)
        m_current = {};
    }
  }
}
