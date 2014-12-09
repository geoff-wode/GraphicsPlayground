#pragma once

#include <string>

namespace Kandy
{
  namespace Renderer
  {
    struct ShaderVertexAttributeType
    {
      enum Enum
      {
        Int,
        Float,
        FloatVector2,
        FloatVector3,
        FloatVector4,
        FloatMatrix4x4
      };
    };

    struct ShaderVertexAttributeLocation
    {
      static const unsigned int Position          = 0;
      static const unsigned int Normal            = 2;  // _NOT_ '1' - see below.
      static const unsigned int TextureCoordinate = 3;
      static const unsigned int Colour            = 4;

      // These allow vertex buffers containing double-precision positions to use the same
      // vertex array.
      static const unsigned int PositionHigh      = Position;
      static const unsigned int PositionLow       = Position + 1;
    };

    struct ShaderVertexAttribute
    {
      ShaderVertexAttributeType::Enum  Type;
      unsigned int  Length;
      unsigned int  Location;
      std::string   Name;
    };
  }
}
