#pragma once

#include <Kandy\renderer\buffers\usagehint.h>

namespace Kandy
{
  namespace Renderer
  {
    class IndexBuffer
    {
    public:
      struct DataType
      {
        enum Enum
        {
          UInt16,
          UInt32
        };
      };

      IndexBuffer(DataType::Enum type, size_t capacity, BufferUsageHint::Enum usageHint);
      ~IndexBuffer();

      // Copy count values from source into the buffer at offset.
      void SetData(const void* const source, size_t count, size_t offset = 0);

      // Copy count values from the offset in the buffer to the destination.
      void GetData(void* const destination, size_t count, size_t offset = 0);

      void Bind();
      static void Unbind();

      DataType::Enum Type() const;
      size_t Capacity() const;
      BufferUsageHint::Enum UsageHint() const;
      size_t SizeInBytes() const;

    private:
      struct Impl;
      Impl* impl;
    };
  }
}
