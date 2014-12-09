#include <renderer\vertexdeclaration.h>

using namespace Kandy;
using namespace Kandy::Renderer;

//------------------------------------------------------

static unsigned int ComputeStride(const VertexElement elements[], size_t count)
{
  unsigned int stride = 0;

  for (size_t i = 0; i < count; ++i)
  {
    stride += elements[i].Size;
  }

  return stride;
}

//------------------------------------------------------

VertexDeclaration::VertexDeclaration(const VertexElement elements[], size_t count)
  : Elements(elements),
    ElementCount(count),
    Stride(ComputeStride(Elements, count))
{
}
