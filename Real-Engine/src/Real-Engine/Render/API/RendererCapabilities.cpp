#include "pch.h"
#include "RendererCapabilities.h"
#include "RendererAPI.h"
#include "Platforms/OpenGl/OpenGlRendererCapabilities.h"

#define REAL_CALL_RIGHT_FN(x) \
    switch(RendererAPI::getAPI())\
    {\
      case RendererAPI::API::OpenGl:\
      {\
        return OpenGlRendererCapabilities::x();\
        break;\
      }\
      case RendererAPI::API::NONE:\
      {\
        return -1;\
        REAL_CORE_ASSERT_MSG(false, "Not supported RenderAPI: 'RenderAPI:NONE' at the moment");\
        break;\
      }\
    }\
    REAL_CORE_ASSERT_MSG(false, "Unknown RendererAPI");\
    return 0;\


namespace Real
{
  int RendererCapabilities::getMaxTextureSize()
  {
    REAL_CALL_RIGHT_FN(getMaxTextureSize);
  }

  int RendererCapabilities::getMaxTextureUints()
  {
    REAL_CALL_RIGHT_FN(getMaxTextureUints);
  }

  int RendererCapabilities::getMaxTextureCoords()
  {
    REAL_CALL_RIGHT_FN(getMaxTextureCoords);
  }

  int RendererCapabilities::getMaxVaryingFloats()
  {
    REAL_CALL_RIGHT_FN(getMaxVaryingFloats);
  }

  int RendererCapabilities::getMaxVertexAttribs()
  {
    REAL_CALL_RIGHT_FN(getMaxVertexAttribs);
  }

  int RendererCapabilities::getMaxElementIndices()
  {
    REAL_CALL_RIGHT_FN(getMaxElementIndices);
  }

  int RendererCapabilities::getMaxVertexUniforms()
  {
    REAL_CALL_RIGHT_FN(getMaxVertexUniforms);
  }

  int RendererCapabilities::getMaxElementVertices()
  {
    REAL_CALL_RIGHT_FN(getMaxElementVertices);
  }

  int RendererCapabilities::getMaxColorAttachments()
  {
    REAL_CALL_RIGHT_FN(getMaxColorAttachments);
  }

  int RendererCapabilities::getMaxFragmentUniforms()
  {
    REAL_CALL_RIGHT_FN(getMaxFragmentUniforms);
  }

  int RendererCapabilities::getMaxRenderBufferSize()
  {
    REAL_CALL_RIGHT_FN(getMaxRenderBufferSize);
  }

  int RendererCapabilities::getMaxUniformBlockSize()
  {
    REAL_CALL_RIGHT_FN(getMaxUniformBlockSize);
  }

  int RendererCapabilities::getMaxTextureBufferSize()
  {
    REAL_CALL_RIGHT_FN(getMaxTextureBufferSize);
  }

  int RendererCapabilities::getMaxTextureImageUnits()
  {
    REAL_CALL_RIGHT_FN(getMaxTextureImageUnits);
  }

  int RendererCapabilities::getMaxVaryingComponents()
  {
    REAL_CALL_RIGHT_FN(getMaxVaryingComponents);
  }

  int RendererCapabilities::getMaxColorTextureSamples()
  {
    REAL_CALL_RIGHT_FN(getMaxColorTextureSamples);
  }
  
  int RendererCapabilities::getMaxDepthTextureSamples()
  {
    REAL_CALL_RIGHT_FN(getMaxDepthTextureSamples);
  }

  int RendererCapabilities::getMaxVertexOutputComponents()
  {
    REAL_CALL_RIGHT_FN(getMaxVertexOutputComponents);
  }

  int RendererCapabilities::getMaxFragmentInputComponents()
  {
    REAL_CALL_RIGHT_FN(getMaxFragmentInputComponents);
  }

  int RendererCapabilities::getMaxVertexUniformBlocks()
  {
    REAL_CALL_RIGHT_FN(getMaxVertexUniformBlocks);
  }
}
