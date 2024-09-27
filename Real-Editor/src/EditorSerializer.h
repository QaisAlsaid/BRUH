#ifndef EDITOR_SERIALIZER_H
#define EDITOR_SERIALIZER_H

#include <Real-Engine/Real-Engine.h>
#include "EditorLayer.h"

namespace Real
{
  class EditorSerializer
  {
  public:
    EditorSerializer() = default;
    EditorSerializer(EditorLayer* e);//const ARef<EditorLayer>& editor);

    void setContext(EditorLayer* e);//const ARef<EditorLayer>& editor);

    bool serialize(const char* path);
    bool deSerialize(const char* path);
  private:
    //ARef<EditorLayer> m_context;
    EditorLayer* m_context;
  };
}



#endif //EDITOR_SERIALIZER_H
