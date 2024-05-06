#ifndef KR_SCENE_SERIALIZER_H
#define KR_SCENE_SERIALIZER_H


#include "Scene.h"

#include <yaml-cpp/yaml.h>


namespace Karen
{
  class SceneSerializer
  {
  public:
    SceneSerializer();
    SceneSerializer(const ARef<Scene>& scene);
    
    void setContext(const ARef<Scene>& scene);

    bool serializeText(const char* path);
    bool deSerializeText(const char* path);
  private:
    void serializeEntity(Entity& e, YAML::Emitter& em);
  private:
    ARef<Scene> m_context;
  };
}


#endif //KR_SCENE_SERIALIZER_H
