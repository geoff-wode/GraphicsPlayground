#pragma once

#include <glm\glm.hpp>
#include <Kandy\renderer\pipelinestate\depthtest.h>
#include <Kandy\renderer\pipelinestate\facetculling.h>

namespace Kandy
{
  namespace Renderer
  {
    struct RasterisationMode
    {
      enum Enum
      {
        Point,
        Line,
        Fill
      };
    };

    /*
      This state controls the fixed-function rendering pipeline, i.e. all the
      states not under the influence of shaders such as the depth test and facet
      culling.
    */
    struct PipelineState
    {
      bool          depthMask;
      glm::bvec4    colourMask;
      DepthTest     depthTest;
      FacetCulling  faceCulling;
      RasterisationMode::Enum rasterisationMode;

      PipelineState()
        : depthMask(true),
          colourMask(true),
          rasterisationMode(RasterisationMode::Fill)
      {
      }
    };
  }
}
