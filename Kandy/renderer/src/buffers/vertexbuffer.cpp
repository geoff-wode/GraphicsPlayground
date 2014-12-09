#include <core\logging.h>
#include <renderer\buffers\vertexbuffer.h>
#include "..\gl_core_3_3.hpp"
#include "bufferusage.h"

using namespace Kandy;
using namespace Kandy::Renderer;

//----------------------------------------------------------------

VertexBuffer::VertexBuffer(size_t vertexCount, const IVertexType& vertexType, BufferUsageHint::Enum usage)
  : SizeInBytes(vertexCount * vertexType.GetVertexDeclaration().Stride),
    VertexCount(vertexCount),
    VertexType(vertexType),
    Usage(usage)
{
  gl::GenBuffers(1, &buffer);
  gl::BufferData(gl::ARRAY_BUFFER, SizeInBytes, NULL, GLBufferUsage(usage));
}

//----------------------------------------------------------------

VertexBuffer::~VertexBuffer()
{
  gl::DeleteBuffers(1, &buffer);
}

//----------------------------------------------------------------

// Write 'elemanentCount' elements of 'data' to the buffer starting at 'offset' bytes.
template <typename T>
void VertexBuffer::Set(const T data[], unsigned int elementCount, unsigned int offsetInBytes)
{
  const unsigned int bytesToWrite = sizeof(T) * elementCount;
  
  if (bytesToWrite + offsetInBytes <= SizeInBytes)
  {
    gl::BindVertexArray(0);
    gl::BindBuffer(gl::ARRAY_BUFFER, buffer);
    gl::BufferSubData(gl::ARRAY_BUFFER, offsetInBytes, sizeToWrite, data);
  }
  else
  {
    ASSERT(false && "out-of-bounds on buffer write");
  }
}

//----------------------------------------------------------------

// Read 'elementCount' elements from the buffer starting at the offset into the array.
template <typename T>
void VertexBuffer::Get(T data[], unsigned int elementCount, unsigned int offsetInBytes)
{
  const unsigned int bytesToRead = sizeof(T) * elementCount;
  
  if (bytesToRead + offsetInBytes <= SizeInBytes)
  {
    gl::BindVertexArray(0);
    gl::BindBuffer(gl::ARRAY_BUFFER, buffer);
    gl::GetBufferSubData(gl::ARRAY_BUFFER, offsetInBytes, bytesToRead, data);
  }
  else
  {
    ASSERT(false && "out-of-bounds on buffer read");
  }
}

//----------------------------------------------------------------

void VertexBuffer::Bind()
{
  gl::BindBuffer(gl::ARRAY_BUFFER, buffer);
}

//----------------------------------------------------------------

void VertexBuffer::Unbind()
{
  gl::BindBuffer(gl::ARRAY_BUFFER, 0);
}
