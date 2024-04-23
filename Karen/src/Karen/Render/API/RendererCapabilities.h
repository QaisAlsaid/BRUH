#ifndef RENDERER_CAPABILITIES_H
#define RENDERER_CAPABILITIES_H


#include <unordered_map>

//TODO: static class and switch in each function to call the right one based on API
namespace Karen
{
  class RendererCapabilities
  {
  public:
    static ARef<RendererCapabilities> create();
    virtual int getMaxTextureSize() = 0;
    virtual int getMaxTextureUints() = 0;
    virtual int getMaxVertexAttribs() = 0;
    virtual int getMaxTextureCoords() = 0;
    virtual int getMaxVertexUniforms() = 0;
    virtual int getMaxFragmentUniforms() = 0;
    virtual int getMaxVaryingFloats() = 0;
    virtual int getMaxElementIndices() = 0;
    virtual int getMaxColorAttachments() = 0;
    virtual int getMaxElementVertices() = 0;
    virtual int getMaxRenderBufferSize() = 0;
    virtual int getMaxUniformBlockSize() = 0;
    virtual int getMaxVaryingComponents() = 0;
    virtual int getMaxTextureBufferSize() = 0;
    virtual int getMaxTextureImageUnits() = 0;
    virtual int getMaxColorTextureSamples() = 0;
    virtual int getMaxDepthTextureSamples() = 0;
    virtual int getMaxVertexUniformBlocks() = 0;
    virtual int getMaxVertexOutputComponents() = 0;
    virtual int getMaxFragmentInputComponents() = 0;
    
    
  };
}


#endif //RENDERER_CAPABILITIES_H
