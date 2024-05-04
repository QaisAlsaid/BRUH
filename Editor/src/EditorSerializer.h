#ifndef KR_EDITOR_SERIALIZER_H
#define KR_EDITOR_SERIALIZER_H

#include <Karen/Karen.h>
#include "EditorLayer.h"

namespace Karen
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



#endif //KR_EDITOR_SERIALIZER_H
