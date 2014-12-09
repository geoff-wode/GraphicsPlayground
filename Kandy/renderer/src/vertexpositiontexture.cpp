#include <renderer\vertexpositiontexture.h>

using namespace Kandy::Renderer;

//-------------------------------------------------------

VertexPositionTexture::VertexPositionTexture()
{
}

//-------------------------------------------------------

const VertexDeclaration& VertexPositionTexture::GetVertexDeclaration() const
{
  static const VertexElement elements[] =
  {
    VertexElement(VertexElement::DataType::Float, 3, offsetof(VertexPositionTexture, Position)),
    VertexElement(VertexElement::DataType::Float, 2, offsetof(VertexPositionTexture, TextureCoord)),
  };
  static const size_t elementCount = sizeof(elements) / sizeof(elements[0]);
  static const VertexDeclaration declaration(elements, elementCount);

  return declaration;
}

bool operator==(const VertexPositionTexture& a, const VertexPositionTexture& b)
{
  return (a.Position == b.Position) && (a.TextureCoord == b.TextureCoord);
}
