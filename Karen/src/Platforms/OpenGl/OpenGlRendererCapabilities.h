#ifndef OPENGL_RENDERER_CAPABILITIES_H
#define OPENGL_RENDERER_CAPABILITIES_H


#include "Karen/Render/API/RendererCapabilities.h"



namespace Karen
{
  class OpenGlRendererCapabilities : public RendererCapabilities
  {
  public:
    int getMaxTextureSize()             override;
    int getMaxTextureUints()            override;
    int getMaxVertexAttribs()           override;
    int getMaxTextureCoords()           override;
    int getMaxVertexUniforms()          override;
    int getMaxFragmentUniforms()        override;
    int getMaxVaryingFloats()           override;
    int getMaxElementIndices()          override;
    int getMaxColorAttachments()        override;
    int getMaxElementVertices()         override;
    int getMaxRenderBufferSize()        override;
    int getMaxUniformBlockSize()        override;
    int getMaxVaryingComponents()       override;
    int getMaxTextureBufferSize()       override;
    int getMaxTextureImageUnits()       override;
    int getMaxColorTextureSamples()     override;
    int getMaxDepthTextureSamples()     override;
    int getMaxVertexUniformBlocks()     override;
    int getMaxVertexOutputComponents()  override;
    int getMaxFragmentInputComponents() override;
  };
}

#endif //OPENGL_RENDERER_CAPABILITIES_H
