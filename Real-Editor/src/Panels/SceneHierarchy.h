#ifndef KR_SCENE_HIERARCHY_H
#define KR_SCENE_HIERARCHY_H

#include <Karen/Karen.h>


namespace Karen
{
  class SceneHierarchy
  {
  public:
    SceneHierarchy() = default;
    SceneHierarchy(const ARef<Scene>& context);

    void setContext(const ARef<Scene>& context);
    inline ARef<Scene> getContext() { return m_context; }
    void onGuiUpdate();
    Entity getCurrentSelected() { return m_current; }
    void clearSelection() { m_current = {};}
  private:
    void drawEntityNode(Entity& entity);
  private:
    ARef<Scene> m_context;//TODO:weakRef
    Entity m_current;
  };
}
#endif //KR_SCENE_HIERARCHY_H
