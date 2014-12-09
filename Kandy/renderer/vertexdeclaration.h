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
      // Constructor will compute the vertex stride based on the elements contained within.
      VertexDeclaration(const VertexElement elements[], size_t count);

      // Number of bytes from one vertex to the next.
      const unsigned int                Stride;

      const VertexElement* const  Elements;
      const size_t                ElementCount;
    };
  }
}
