#pragma once

#include <glm\glm.hpp>
#include <Kandy\renderer\vertextype.h>
#include <Kandy\renderer\vertexdeclaration.h>

namespace Kandy
{
  namespace Renderer
  {
#pragma pack(push, 1)
    struct VertexPositionTexture : public IVertexType
    {
      glm::vec3 Position;
      glm::vec2 TextureCoord;

      VertexPositionTexture();
      VertexPositionTexture(const glm::vec3& position, const glm::vec2& textureCoord);

      // IVertexType:
      const VertexDeclaration& GetVertexDeclaration() const;
    };
#pragma pack(pop)

    bool operator==(const VertexPositionTexture& a, const VertexPositionTexture& b);
    bool operator!=(const VertexPositionTexture& a, const VertexPositionTexture& b) { return !(a == b); }
  }
}
