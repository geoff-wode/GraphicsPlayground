#pragma once

#include <glm\glm.hpp>

namespace Kandy
{
  namespace Renderer
  {
    /*
    Pipeline state contains the configuration for the fixed-function aspects of the renderer, such as
    whether the depth test is enabled, etc.
    */
    struct PipelineState
    {
      glm::bvec4  colourMask;

      PipelineState()
        : colourMask(true,true,true,true)
      {
      }
    };
  }
}
