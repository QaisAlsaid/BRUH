#ifndef OPENGL_CORE_H
#define OPENGL_CORE_H

#ifndef REAL_EMSCRIPTEN
#include <glad/glad.h>
#define GLES 0
#else 
#include <GLES3/gl32.h>
#define GLES 1
#endif //REAL_EMSCRIPTEN

#define glCall(x) x; \
  glPrintErr(#x, __FILE__, __LINE__);
void glPrintErr(const char* fn, const char* file, int line);



#endif //OPENGL_CORE_H
