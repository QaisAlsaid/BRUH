#ifndef RENDERER_CAPABILITIES_H
#define RENDERER_CAPABILITIES_H



namespace Karen
{
  class RendererCapabilities
  {
  public:
    static int getMaxTextureSize();
    static int getMaxTextureUints();
    static int getMaxVertexAttribs();
    static int getMaxTextureCoords();
    static int getMaxVertexUniforms();
    static int getMaxFragmentUniforms();
    static int getMaxVaryingFloats();
    static int getMaxElementIndices();
    static int getMaxColorAttachments();
    static int getMaxElementVertices();
    static int getMaxRenderBufferSize();
    static int getMaxUniformBlockSize();
    static int getMaxVaryingComponents();
    static int getMaxTextureBufferSize();
    static int getMaxTextureImageUnits();
    static int getMaxColorTextureSamples();
    static int getMaxDepthTextureSamples();
    static int getMaxVertexUniformBlocks();
    static int getMaxVertexOutputComponents();
    static int getMaxFragmentInputComponents();
    
    
  };
}


#endif //RENDERER_CAPABILITIES_H
