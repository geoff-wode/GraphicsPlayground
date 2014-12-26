#pragma once

#include <string>

namespace Kandy
{
  namespace Renderer
  {
    struct ShaderVertexAttribType
    {
      enum Enum
      {
        Float,
        FloatVector2,
        FloatVector3,
        FloatVector4,
        FloatMatrix22,
        FloatMatrix33,
        FloatMatrix44,
        Int,
        IntVector2,
        IntVector3,
        IntVector4
      };
    };

    struct ShaderVertexAttribute
    {
      ShaderVertexAttribute(const char* name, unsigned int location, unsigned int length, ShaderVertexAttribType::Enum type)
        : Name(name),
          Location(location),
          Length(length),
          Type(type)
      {
      }

      const std::string Name;
      const unsigned int      Location;
      const unsigned int      Length;
      const ShaderVertexAttribType::Enum  Type;
    };
  }
}
