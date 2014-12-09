#pragma once

#include <glm\glm.hpp>

namespace Kandy
{
  namespace Renderer
  {
    namespace Buffers
    {
      enum Enum
      {
        Colour  = 1 << 0,
        Depth   = 1 << 1,
        Stencil = 1 << 2,
        ColourDepth = Colour | Depth,
        All         = Colour | Depth | Stencil
      };
    }

    struct ClearState
    {
      Buffers::Enum buffers;
      glm::vec4   colour;
      glm::bvec4  colourMask;

      ClearState()
        : buffers(Buffers::All),
          colour(0,0,0,0),
          colourMask(true,true,true,true)
      {
      }
    };
  }
}
