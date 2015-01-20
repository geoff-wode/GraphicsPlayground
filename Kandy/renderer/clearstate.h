#pragma once

#include <glm\glm.hpp>

namespace Kandy
{
  namespace Renderer
  {
    /**
      @brief A collection of states which affect clearing the clearing of buffers.

      This state is passed to all @see Context::Clear calls.
      */
    struct ClearState
    {
      /**
        @brief Specifies which buffers will be affected by @see Context::Clear.
        */
      struct Buffers
      {
        enum Enum
        {
          Colour  = 1,
          Depth   = 2,
          Stencil = 4,
          ColourDepth   = Colour | Depth,
          DepthStencil  = Depth | Stencil,
          All           = Colour | Depth | Stencil
        };
      };

      ClearState()
        : colourMask(true),
          depthMask(true),
          depth(1.0f),
          colour(0.0f)
      {
      }

      glm::bvec4  colourMask; /** Clear colour buffer components or not. */
      bool      depthMask;    /** Clear depth buffer or not. */

      double    depth;  /** Value to set in the depth buffer. */
      glm::vec4 colour; /** Value to set in the colour buffer. */

      Buffers::Enum buffers;
    };
  }
}
