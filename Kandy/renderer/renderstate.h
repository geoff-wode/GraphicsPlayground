#pragma once

#include <glm\glm.hpp>
#include <boost\shared_ptr.hpp>

/**
  @file context.h

  */

namespace Kandy
{
  namespace Renderer
  {
    struct Shader;
    struct VertexArray;

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
