#pragma once

#include <Kandy\renderer\pipelinestate\windingorder.h>

namespace Kandy
{
  namespace Renderer
  {
    struct Face
    {
      enum Enum
      {
        Front,
        Back,
        FrontAndBack
      };
    };

    struct FacetCulling
    {
      bool        enabled;
      Face::Enum  cullFace;
      WindingOrder::Enum frontFaceWindingOrder;

      FacetCulling()
        : enabled(true),
          cullFace(Face::Back),
          frontFaceWindingOrder(WindingOrder::CounterClockwise)
      {
      }
    };
  }
}
