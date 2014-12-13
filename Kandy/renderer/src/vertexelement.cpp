#include <renderer\vertexelement.h>

using namespace Kandy::Renderer;

//-----------------------------------------------------------

VertexElement::VertexElement(DataType::Enum type, unsigned int elementCount, unsigned int offset)
  : Type(type),
    ComponentCount(elementCount),
    Offset(offset),
    Size(elementCount * VertexElementSize(type)),
    Normalise(false)
{
}

//-----------------------------------------------------------

VertexElement::VertexElement(DataType::Enum type, unsigned int elementCount, unsigned int offset, bool normalise)
  : Type(type),
    ComponentCount(elementCount),
    Offset(offset),
    Size(elementCount * VertexElementSize(type)),
    Normalise(normalise)
{
}

//-----------------------------------------------------------

bool Kandy::Renderer::operator==(const VertexElement& a, const Renderer::VertexElement& b)
{
  return  (a.Type == b.Type)
      &&  (a.ComponentCount == b.ComponentCount)
      &&  (a.Offset == b.Offset)
      &&  (a.Size == b.Size);
}

//-----------------------------------------------------------

bool Kandy::Renderer::operator!=(const VertexElement& a, const Renderer::VertexElement& b)
{
  return !(a == b);
}

//-----------------------------------------------------------

unsigned int Kandy::Renderer::VertexElementSize(VertexElement::DataType::Enum type)
{
  switch (type)
  {
  case VertexElement::DataType::Byte:      return sizeof(uint8_t); break;
  case VertexElement::DataType::Short:     return sizeof(uint16_t); break;
  case VertexElement::DataType::Int:       return sizeof(uint32_t); break;
  case VertexElement::DataType::Float:     return sizeof(float); break;
  case VertexElement::DataType::HalfFloat: return sizeof(float) / 2; break;
  case VertexElement::DataType::Double:    return sizeof(double); break;
  }
  return 0;
}
