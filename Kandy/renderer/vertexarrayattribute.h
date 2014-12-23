#pragma once

namespace Kandy
{
  namespace Renderer
  {
    struct ComponentType
    {
      enum Enum
      {
        Byte,
        UnsignedByte,
        Short,
        UnsignedShort,
        Int,
        UnsignedInt,
        Float,
        HalfFloat,
      };
    };

    struct VertexArrayAttribute
    {
      size_t  componentCount;
      size_t  offset;
      ComponentType::Enum type;
      bool    normalise;
      size_t  SizeInBytes() const;
    };
  }
}
