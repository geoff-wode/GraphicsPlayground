#pragma once

#include <Kandy\renderer\shaders\shader.h>
#include <Kandy\renderer\pipelinestate.h>
#include <Kandy\renderer\vertexarray.h>
#include <Kandy\renderer\buffers\indexbuffer.h>

namespace Kandy
{
  namespace Renderer
  {
    struct PrimitiveType
    {
      enum Enum
      {
        Points,
        Lines,
        LineLoop,
        LineStrip,
        Triangles,
        TriangleStrip,
        TriangleFan,
        LinesAdjacency,
        LineStripAdjacency,
        TrianglesAdjacency,
        TriangleStripAdjacency
      };
    };

    /*
      A complete render state is passed to each call to Device::Render to define
      the complete state of the GPU for rendering.
    */
    struct RenderState
    {
      Shader*       shader;
      VertexArray*  vertexArray;
      IndexBuffer*  indexBuffer;
      PipelineState pipelineState;
    };
  }
}
