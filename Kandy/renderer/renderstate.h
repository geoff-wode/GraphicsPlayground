#pragma once

#include <Kandy\renderer\pipelinestate.h>
#include <Kandy\renderer\shader.h>
#include <Kandy\renderer\buffers\indexbuffer.h>
#include <Kandy\renderer\buffers\vertexbuffer.h>

namespace Kandy
{
  namespace Renderer
  {
    struct RenderState
    {
      Shader::Ptr shader;
      VertexBuffer::Ptr vertexBuffer;
      IndexBuffer::Ptr  indexBuffer;
      PipelineState pipelineState;
    };
  }
}
