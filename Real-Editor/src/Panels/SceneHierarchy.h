#ifndef SCENE_HIERARCHY_H
#define SCENE_HIERARCHY_H

#include <Real-Engine/Real-Engine.h>


namespace Real
{
  class SceneHierarchy
  {
  public:
    SceneHierarchy() = default;
    SceneHierarchy(const ARef<Scene>& context);

    void setContext(const ARef<Scene>& context);
    inline ARef<Scene> getContext() { return m_context; }
    void onGuiUpdate();
    inline Entity getCurrentSelected() { return m_current; }
    void setCurrentSelected(Entity e) {m_current = e;}
    void clearSelection() { m_current = {};}
  private:
    void drawEntityNode(Entity& entity);
  private:
    ARef<Scene> m_context;//TODO:weakRef
    Entity m_current;
  };
}
#endif //SCENE_HIERARCHY_H
