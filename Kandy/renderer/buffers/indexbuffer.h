#pragma once

#include <boost\shared_ptr.hpp>
#include <Kandy\renderer\buffers\usagehint.h>

namespace Kandy
{
  namespace Renderer
  {
    class IndexBuffer
    {
    public:
      typedef boost::shared_ptr<IndexBuffer> Ptr;

      struct BufferType
      {
        enum Enum
        {
          UnsignedShort,
          UnsignedInt
        };
      };

      IndexBuffer(size_t elementCount, BufferType::Enum type, BufferUsageHint::Enum usage);
      ~IndexBuffer();

      // Write 'elemanentCount' elements of 'data' to the buffer starting at 'offset' bytes.
      template <typename T>
      void Set(const T data[], unsigned int elementCount, unsigned int offsetInBytes);

      // Read 'elementCount' elements from the buffer starting at the offset into the array.
      template <typename T>
      void Get(T data[], unsigned int elementCount, unsigned int offsetInBytes);

      void Bind();
      static void Unbind();

      const size_t SizeInBytes;
      const size_t IndexCount;
      const BufferType::Enum      Type;
      const BufferUsageHint::Enum Usage;

    private:
      unsigned int buffer;
    };
  }
}
