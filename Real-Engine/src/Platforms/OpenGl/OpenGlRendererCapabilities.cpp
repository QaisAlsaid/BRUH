#include "pch.h"
#include "OpenGlCore.h"
#include "OpenGlRendererCapabilities.h"

#define GL_GET_THEN_RETURN_INT(x) \
  int res = -1; \
  glCall(glGetIntegerv(x, (GLint*)&res)); \
  return res;


namespace Real
{
  int OpenGlRendererCapabilities::getMaxTextureSize()
  {
    GL_GET_THEN_RETURN_INT(GL_MAX_RECTANGLE_TEXTURE_SIZE);
  }

  int OpenGlRendererCapabilities::getMaxTextureUints()
  {
    GL_GET_THEN_RETURN_INT(GL_MAX_TEXTURE_UNITS);
  }

  int OpenGlRendererCapabilities::getMaxTextureCoords()
  {
    GL_GET_THEN_RETURN_INT(GL_MAX_TEXTURE_COORDS);
  }

  int OpenGlRendererCapabilities::getMaxVaryingFloats()
  {
    GL_GET_THEN_RETURN_INT(GL_MAX_VARYING_FLOATS);
  }
  int OpenGlRendererCapabilities::getMaxVertexAttribs()
  {
    GL_GET_THEN_RETURN_INT(GL_MAX_VERTEX_ATTRIBS);
  }

  int OpenGlRendererCapabilities::getMaxVertexUniforms()
  {
    GL_GET_THEN_RETURN_INT(GL_MAX_VERTEX_UNIFORM_BLOCKS);
  }

  int OpenGlRendererCapabilities::getMaxFragmentUniforms()
  {
    GL_GET_THEN_RETURN_INT(GL_MAX_FRAGMENT_UNIFORM_BLOCKS);
  }

  int OpenGlRendererCapabilities::getMaxElementIndices()
  {
    GL_GET_THEN_RETURN_INT(GL_MAX_ELEMENTS_INDICES);
  }
  
  int OpenGlRendererCapabilities::getMaxColorAttachments()
  {
    GL_GET_THEN_RETURN_INT(GL_MAX_COLOR_ATTACHMENTS);
  }
  
  int OpenGlRendererCapabilities::getMaxElementVertices()
  {
    GL_GET_THEN_RETURN_INT(GL_MAX_ELEMENTS_VERTICES);
  }
  
  int OpenGlRendererCapabilities::getMaxRenderBufferSize()
  {
    GL_GET_THEN_RETURN_INT(GL_MAX_RENDERBUFFER_SIZE);
  }
  
  int OpenGlRendererCapabilities::getMaxUniformBlockSize()
  {
    GL_GET_THEN_RETURN_INT(GL_MAX_UNIFORM_BLOCK_SIZE);
  }
  
  int OpenGlRendererCapabilities::getMaxVaryingComponents()
  {
    GL_GET_THEN_RETURN_INT(GL_MAX_VARYING_COMPONENTS);
  }
  
  int OpenGlRendererCapabilities::getMaxTextureBufferSize()
  {
    GL_GET_THEN_RETURN_INT(GL_MAX_TEXTURE_BUFFER_SIZE);
  }
  
  int OpenGlRendererCapabilities::getMaxTextureImageUnits()
  {
    GL_GET_THEN_RETURN_INT(GL_MAX_TEXTURE_IMAGE_UNITS);
  }

  int OpenGlRendererCapabilities::getMaxColorTextureSamples()
  {
    GL_GET_THEN_RETURN_INT(GL_MAX_COLOR_TEXTURE_SAMPLES);
  }
  
  int OpenGlRendererCapabilities::getMaxDepthTextureSamples()
  {
    GL_GET_THEN_RETURN_INT(GL_MAX_DEPTH_TEXTURE_SAMPLES);
  }
  
  int OpenGlRendererCapabilities::getMaxVertexUniformBlocks()
  {
    GL_GET_THEN_RETURN_INT(GL_MAX_VERTEX_UNIFORM_BLOCKS);
  }
  
  int OpenGlRendererCapabilities::getMaxVertexOutputComponents()
  {
    GL_GET_THEN_RETURN_INT(GL_MAX_VERTEX_OUTPUT_COMPONENTS);
  }
  
  int OpenGlRendererCapabilities::getMaxFragmentInputComponents()
  {
    GL_GET_THEN_RETURN_INT(GL_MAX_FRAGMENT_INPUT_COMPONENTS);
  }
}
