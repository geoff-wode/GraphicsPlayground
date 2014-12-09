#include <renderer\vertexdeclaration.h>

using namespace Kandy;
using namespace Kandy::Renderer;

//------------------------------------------------------

static unsigned int ComputeStride(const std::vector<VertexElement>& elements)
{
  unsigned int stride = 0;

  for (size_t i = 0; i < elements.size(); ++i)
  {
    stride += elements[i].Size;
  }

  return stride;
}

//------------------------------------------------------

VertexDeclaration::VertexDeclaration(const VertexElement elements[], size_t count)
  : Elements(elements, elements + count),
    Stride(ComputeStride(Elements))
{
}

//------------------------------------------------------

VertexDeclaration::VertexDeclaration(const std::vector<VertexElement> elements)
  : Elements(elements),
    Stride(ComputeStride(Elements))
{
}

