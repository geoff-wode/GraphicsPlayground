#pragma once

#include <vector>
#include <Kandy\renderer\vertexelement.h>

namespace Kandy
{
  namespace Renderer
  {
    /*
      Defines per vertex data.
    */
    struct VertexDeclaration
    {
      // Constructor(s) will compute the vertex stride based on the elements contained within.
      VertexDeclaration(const VertexElement elements[], size_t count);
      VertexDeclaration(const std::vector<VertexElement> elements);

      // Number of bytes from one vertex to the next.
      const unsigned int                Stride;

      const std::vector<VertexElement>  Elements;
    };
  }
}
