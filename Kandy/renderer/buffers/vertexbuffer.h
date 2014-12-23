#pragma once

#include <Kandy\renderer\buffers\usagehint.h>

namespace Kandy
{
  namespace Renderer
  {
    class VertexBuffer
    {
    public:
      VertexBuffer(size_t sizeInBytes, BufferUsageHint::Enum usageHint);
      ~VertexBuffer();

      // Copy count values from source into the buffer at offset.
      void SetData(const void* const source, size_t sizeInBytes, size_t offset = 0);

      // Copy count values from the offset in the buffer to the destination.
      void GetData(void* const destination, size_t sizeInBytes, size_t offset = 0);

      void Bind();
      static void Unbind();

      BufferUsageHint::Enum UsageHint() const;
      size_t SizeInBytes() const;

    private:
      struct Impl;
      Impl* impl;
    };
  }
}
