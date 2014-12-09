#pragma once

#include <Kandy\renderer\vertexdeclaration.h>

namespace Kandy
{
  namespace Renderer
  {
    struct IVertexType
    {
      virtual const VertexDeclaration& GetVertexDeclaration() const = 0;
    };
  }
}
