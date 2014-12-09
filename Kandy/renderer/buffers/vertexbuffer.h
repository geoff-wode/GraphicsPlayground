#pragma once

#include <boost\shared_ptr.hpp>
#include <Kandy\renderer\buffers\usagehint.h>
#include <Kandy\renderer\vertextype.h>

namespace Kandy
{
  namespace Renderer
  {
    class VertexBuffer
    {
    public:
      typedef boost::shared_ptr<VertexBuffer> Ptr;

      /*
        Create a buffer which can hold up to vertexCount vertices of the given type.
      */
      VertexBuffer(size_t vertexCount, const IVertexType& vertexType, BufferUsageHint::Enum usage);
      ~VertexBuffer();

      // Write 'elemanentCount' elements of 'data' to the buffer starting at 'offset' bytes.
      template <typename T>
      void Set(const T data[], unsigned int elementCount, unsigned int offsetInBytes);

      // Read 'elementCount' elements from the buffer starting at the offset into the array.
      template <typename T>
      void Get(T data[], unsigned int elementCount, unsigned int offsetInBytes);

      void Bind();
      static void Unbind();

      const size_t SizeInBytes;
      const size_t VertexCount;
      const IVertexType& VertexType;
      const BufferUsageHint::Enum Usage;

    private:
      unsigned int buffer;
    };
  }
}
