#ifndef SCENE_EVENTS_H
#define SCENE_EVENTS_H

#include "EditorEvents.h"


namespace Real
{
  enum class SceneState 
  {
    Play, Stop
  };

  class SceneSetEvent : public EditorEvent 
  {
  public:
    SceneSetEvent(const ARef<Scene>& scene) : scene(scene) {}

    inline ARef<Scene> getScene() { return scene; }
  
    
    inline int getCategory() const override { return EditorEventCategory::ECATSCENE; }

    std::string toString() const override
    {
      std::stringstream ss;
      ss << "SceneSetEvent:" << scene;
      return ss.str();
    }
    
    static  EditorEventType  getStaticType()                { return EditorEventType::SceneChanged; }
    inline  EditorEventType  getEventType()  const override { return getStaticType();               }
    inline  std::string      getName()       const override { return "SceneSetEvent";           }

  private:
    ARef<Scene> scene;
  };

  class SceneChangedEvent : public EditorEvent 
  {
  public:
    SceneChangedEvent(const ARef<Scene>& old, const ARef<Scene>& _new) 
      : new_scene(_new), old_scene(old) {}

    inline ARef<Scene> getOldScene() { return old_scene; }
    inline ARef<Scene> getNewScene() { return new_scene; }

    inline int getCategory() const override { return EditorEventCategory::ECATSCENE; }

    std::string toString() const override
    {
      std::stringstream ss;
      ss << "SceneChangedEvent: [ OldScene: " << old_scene << ", " << "NewScene: " << new_scene << " ]";
      return ss.str();
    }
    
    static  EditorEventType  getStaticType()                { return EditorEventType::SceneChanged; }
    inline  EditorEventType  getEventType()  const override { return getStaticType();               }
    inline  std::string      getName()       const override { return "SceneChangedEvent";           }
  protected:
    ARef<Scene> new_scene, old_scene;
  };

  class ScenePlayEvent : public SceneChangedEvent
  {
  public:
    ScenePlayEvent(const ARef<Scene>& editor_scene, const ARef<Scene>& runtime_scene)
      : SceneChangedEvent(editor_scene, runtime_scene) {}

    inline ARef<Scene> getEditorScene()  { return old_scene; }
    inline ARef<Scene> getRuntimeScene() { return new_scene; }

    
    inline int getCategory() const override { return EditorEventCategory::ECATSCENE; }
    
    std::string toString() const override
    {
      std::stringstream ss;
      ss << "ScenePlayEvent: [ EditorScene: " << old_scene << ", " << "RuntimeScene: " << new_scene << " ]";
      return ss.str();
    }
    
    static  EditorEventType  getStaticType()               { return EditorEventType::ScenePlay; }
    EditorEventType          getEventType() const override { return getStaticType();            }
    std::string              getName()      const override { return "ScenePlayEvent";           }
  };

  class SceneStopEvent : public SceneChangedEvent
  {
  public:
    SceneStopEvent(const ARef<Scene>& editor_scene, const ARef<Scene>& runtime_scene) 
      : SceneChangedEvent(runtime_scene, editor_scene) {}

    inline ARef<Scene> getEditorScene()  { return old_scene; }
    inline ARef<Scene> getRuntimeScene() { return new_scene; }


    std::string toString() const override
    {
      std::stringstream ss;
      ss << "SceneStopEvent: [ EditorScene: " << new_scene << ", " << "RuntimeScene: " << old_scene << " ]";
      return ss.str();
    }
    
    static  EditorEventType  getStaticType()               { return EditorEventType::SceneStop; }
    EditorEventType          getEventType() const override { return getStaticType();            }
    std::string              getName()      const override { return "SceneStopEvent";           }

  };

}

#endif //SCENE_EVENTS_H
