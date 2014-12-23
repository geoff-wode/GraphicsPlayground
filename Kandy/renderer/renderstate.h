#pragma once

#include <Kandy\renderer\vertexarray.h>
#include <Kandy\renderer\buffers\indexbuffer.h>
#include <Kandy\renderer\pipelinestate.h>

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
      // TODO: Add shader

      VertexArray* vertexArray;
      IndexBuffer* indexBuffer;
      PipelineState pipelineState;
    };
  }
}
