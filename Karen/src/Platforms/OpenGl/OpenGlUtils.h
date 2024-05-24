#ifndef OPENGL_UTILS_H
#define OPENGL_UTILS_H

#include "OpenGlCore.h"

#include "Karen/Render/API/Texture.h"
#include "Karen/Render/API/FrameBuffer.h"


namespace Karen
{
  namespace OpenGlUtils
  {
    GLenum toOpenGlFilters(Texture::FilterType);
    GLenum toOpenGlWrapModes(Texture::WrapMode);
    GLenum toOpenGlFormat(FrameBuffer::TextureFormat);
    GLenum toOpenGlFormat(FrameBuffer::TextureInternalFormat);
    GLuint genTextures(int samples = 1);
    void bindTexture(GLuint id, int samples);
    void applayFilters(const Texture::Filters& fils);
  }
}

#endif //OPENGL_UTILS_H
