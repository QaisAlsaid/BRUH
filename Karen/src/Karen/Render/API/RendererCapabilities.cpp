#include "pch.h"
#include "RendererCapabilities.h"
#include "RendererAPI.h"
#include "Platforms/OpenGl/OpenGlRendererCapabilities.h"

#define KAREN_CALL_RIGHT_FN(x) \
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
        KAREN_CORE_ASSERT_MSG(false, "Not supported RenderAPI: 'RenderAPI:NONE' at the moment");\
        break;\
      }\
    }\
    KAREN_CORE_ASSERT_MSG(false, "Unknown RendererAPI");\
    return 0;\


namespace Karen
{
  int RendererCapabilities::getMaxTextureSize()
  {
    KAREN_CALL_RIGHT_FN(getMaxTextureSize);
  }

  int RendererCapabilities::getMaxTextureUints()
  {
    KAREN_CALL_RIGHT_FN(getMaxTextureUints);
  }

  int RendererCapabilities::getMaxTextureCoords()
  {
    KAREN_CALL_RIGHT_FN(getMaxTextureCoords);
  }

  int RendererCapabilities::getMaxVaryingFloats()
  {
    KAREN_CALL_RIGHT_FN(getMaxVaryingFloats);
  }

  int RendererCapabilities::getMaxVertexAttribs()
  {
    KAREN_CALL_RIGHT_FN(getMaxVertexAttribs);
  }

  int RendererCapabilities::getMaxElementIndices()
  {
    KAREN_CALL_RIGHT_FN(getMaxElementIndices);
  }

  int RendererCapabilities::getMaxVertexUniforms()
  {
    KAREN_CALL_RIGHT_FN(getMaxVertexUniforms);
  }

  int RendererCapabilities::getMaxElementVertices()
  {
    KAREN_CALL_RIGHT_FN(getMaxElementVertices);
  }

  int RendererCapabilities::getMaxColorAttachments()
  {
    KAREN_CALL_RIGHT_FN(getMaxColorAttachments);
  }

  int RendererCapabilities::getMaxFragmentUniforms()
  {
    KAREN_CALL_RIGHT_FN(getMaxFragmentUniforms);
  }

  int RendererCapabilities::getMaxRenderBufferSize()
  {
    KAREN_CALL_RIGHT_FN(getMaxRenderBufferSize);
  }

  int RendererCapabilities::getMaxUniformBlockSize()
  {
    KAREN_CALL_RIGHT_FN(getMaxUniformBlockSize);
  }

  int RendererCapabilities::getMaxTextureBufferSize()
  {
    KAREN_CALL_RIGHT_FN(getMaxTextureBufferSize);
  }

  int RendererCapabilities::getMaxTextureImageUnits()
  {
    KAREN_CALL_RIGHT_FN(getMaxTextureImageUnits);
  }

  int RendererCapabilities::getMaxVaryingComponents()
  {
    KAREN_CALL_RIGHT_FN(getMaxVaryingComponents);
  }

  int RendererCapabilities::getMaxColorTextureSamples()
  {
    KAREN_CALL_RIGHT_FN(getMaxColorTextureSamples);
  }
  
  int RendererCapabilities::getMaxDepthTextureSamples()
  {
    KAREN_CALL_RIGHT_FN(getMaxDepthTextureSamples);
  }

  int RendererCapabilities::getMaxVertexOutputComponents()
  {
    KAREN_CALL_RIGHT_FN(getMaxVertexOutputComponents);
  }

  int RendererCapabilities::getMaxFragmentInputComponents()
  {
    KAREN_CALL_RIGHT_FN(getMaxFragmentInputComponents);
  }

  int RendererCapabilities::getMaxVertexUniformBlocks()
  {
    KAREN_CALL_RIGHT_FN(getMaxVertexUniformBlocks);
  }
}
