#pragma once

#include <glm\glm.hpp>
#include <boost\shared_ptr.hpp>

/**
  @file renderstate.h

  */

namespace Kandy
{
  namespace Renderer
  {
    struct Shader;
    struct VertexArray;

    /**
      @brief A render state is the complete set of parameters required to describe the context for rendering.

      Applications provide a complete render state to every @see Device::Render call, which then manages
      updating only those GPU states which are different from call to call, minimising GPU state changes.
      */
    struct RenderState
    {
      RenderState();

      bool        depthMask;
      glm::bvec4  colourMask;

      boost::shared_ptr<Shader> shader;
      boost::shared_ptr<VertexArray> vertexArray;
    };
  }
}
