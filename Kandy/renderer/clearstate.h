#pragma once

#include <glm\glm.hpp>

namespace Kandy
{
  namespace Renderer
  {
    struct ClearState
    {
      struct Buffers
      {
        enum Enum
        {
          Colour  = 1,
          Depth   = 2,
          Stencil = 4,
          ColourDepth = Colour | Depth,
          All = Colour | Depth | Stencil
        };
      };

      struct DepthBuffer
      {
        DepthBuffer() : mask(true), value(1.0f) { }
        bool mask;
        float value;
      };

      struct ColourBuffer
      {
        ColourBuffer() : mask(true), colour(0) { }
        glm::bvec4 mask;
        glm::vec4  colour;
      };
      
      DepthBuffer   depthBuffer;
      ColourBuffer  colourBuffer;
    };
  }
}
