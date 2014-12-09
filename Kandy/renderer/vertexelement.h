#pragma once

#include <stdint.h>

namespace Kandy
{
  namespace Renderer
  {
    /*
      Defines one element in a vertex data buffer.
    */
    struct VertexElement
    {
      struct DataType
      {
        enum Enum
        {
          Byte,
          Short,
          Int,
          Float,
          HalfFloat,
          Double
        };
      };

      /*
      Example usage:
        struct MyVertex
        {
          glm::vec3 position;
          float     combobulationFactor;
        };
        const VertexElement positionElement(DataType::Float, 3, offsetof(MyVertex, position));
        const VertexElement combobulationElement(DataType::Float, 1, offsetof(MyVertex, combobulationFactor));
      */
      VertexElement(DataType::Enum type, unsigned int elementCount, unsigned int offset);

      const DataType::Enum  Type;
      const unsigned int    ElementCount;
      const unsigned int    Offset;
      const unsigned int    Size;
    };

    bool operator==(const VertexElement& a, const Renderer::VertexElement& b);
    bool operator!=(const VertexElement& a, const Renderer::VertexElement& b);

    unsigned int VertexElementSize(VertexElement::DataType::Enum type);
  }
}
