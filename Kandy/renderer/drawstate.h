#pragma once

#include <Kandy\renderer\pipelinestate.h>
#include <Kandy\renderer\shader.h>

namespace Kandy
{
  namespace Renderer
  {
    struct RenderState
    {
      Shader::Ptr shader;
      PipelineState pipelineState;
    };
  }
}
