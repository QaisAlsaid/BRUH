#ifndef OPENGL_CORE_H
#define OPENGL_CORE_H

#ifndef KAREN_EMSCRIPTEN
#include <glad/glad.h>
#else 
#include <GLES3/gl32.h>
#endif //KAREN_EMSCRIPTEN

#define glCall(x) x; \
  glPrintErr(#x, __FILE__, __LINE__);
void glPrintErr(const char* fn, const char* file, int line);


#endif //OPENGL_CORE_H
