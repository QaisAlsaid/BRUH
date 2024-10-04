#include "SceneHierarchy.h"
#include "EditorEvents/EditorEventsManager.h"
#include "EditorEvents/SceneHierarchyPanelEvents.h"
#include "Real-Engine/Core/Macros.h"
#include "Real-Engine/Scene/Components.h"
#include "imgui.h"


namespace Real
{
  SceneHierarchy::SceneHierarchy()
  {
    m_sub_id = EditorEventsManager::subscribe(BIND_EVENT_FUNCTION(SceneHierarchy::onEditorEvent));
  }

  SceneHierarchy::SceneHierarchy(const ARef<Scene>& context)
  {
    m_sub_id = EditorEventsManager::subscribe(BIND_EVENT_FUNCTION(SceneHierarchy::onEditorEvent));
    setContext(context);
  }

  void SceneHierarchy::setContext(const ARef<Scene>& context)
  {
    SCHPContextChangedEvent e(context);
    EditorEventsManager::onEvent(e);
    m_context = context;
  }
  
  void SceneHierarchy::setCurrentSelected(Entity e)
  {
    m_current = e;
    SCHPSelectionChangedEvent ev(e);
    EditorEventsManager::onEvent(ev);
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
      ImGui::Begin("__DEBUG__");
      ImGui::Text("id: %u", e);
      ImGui::End();

    });
    
    if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
    {
      setCurrentSelected({});
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
      setCurrentSelected(e);
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
        setCurrentSelected({});
    }
  }

  void SceneHierarchy::onEditorEvent(EditorEvent& e)
  {
    EditorEventDispatcher dp(e);
    dp.dispatch<SceneChangedEvent>(BIND_EVENT_FUNCTION(SceneHierarchy::onSceneChangedCall));
    dp.dispatch<MousePickedChangedEvent>(BIND_EVENT_FUNCTION(SceneHierarchy::onMousePickedChangedCall));
  }

  bool SceneHierarchy::onSceneChangedCall(SceneChangedEvent& sc)
  {
    setContext(sc.getNewScene());
    setCurrentSelected({});
    return false;
  }

  bool SceneHierarchy::onMousePickedChangedCall(MousePickedChangedEvent& mpc)
  {
    setCurrentSelected(mpc.getEntity());
    return false;
  }

  bool SceneHierarchy::onSceneSetCall(SceneSetEvent& ss)
  {
    setContext(ss.getScene());
    setCurrentSelected({});
    return false;
  }
}
