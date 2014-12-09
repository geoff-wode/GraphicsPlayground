#include <stdint.h>
#include <core\logging.h>
#include <renderer\buffers\indexbuffer.h>
#include "..\gl_core_3_3.hpp"
#include "bufferusage.h"

using namespace Kandy;
using namespace Kandy::Renderer;

//----------------------------------------------------------------

static size_t SizeOf(IndexBuffer::BufferType::Enum type)
{
  switch (type)
  {
  case IndexBuffer::BufferType::UnsignedShort:  return sizeof(uint16_t); break;
  case IndexBuffer::BufferType::UnsignedInt:    return sizeof(uint32_t); break;
  default: ASSERT(false);
  }
  return 0;
}

//----------------------------------------------------------------

IndexBuffer::IndexBuffer(size_t elementCount, BufferType::Enum type, BufferUsageHint::Enum usage)
  : SizeInBytes(elementCount * SizeOf(type)),
    IndexCount(elementCount),
    Type(type),
    Usage(usage)
{
  gl::GenBuffers(1, &buffer);
  gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, SizeInBytes, NULL, GLBufferUsage(usage));
}

//----------------------------------------------------------------

IndexBuffer::~IndexBuffer()
{
  gl::DeleteBuffers(1, &buffer);
}

//----------------------------------------------------------------

// Write 'elemanentCount' elements of 'data' to the buffer starting at 'offset' bytes.
template <typename T>
void IndexBuffer::Set(const T data[], unsigned int elementCount, unsigned int offsetInBytes)
{
  const unsigned int bytesToWrite = sizeof(T) * elementCount;
  
  if (bytesToWrite + offsetInBytes <= SizeInBytes)
  {
    gl::BindVertexArray(0);
    gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, buffer);
    gl::BufferSubData(gl::ELEMENT_ARRAY_BUFFER, offsetInBytes, sizeToWrite, data);
  }
  else
  {
    ASSERT(false && "out-of-bounds on buffer write");
  }
}

//----------------------------------------------------------------

// Read 'elementCount' elements from the buffer starting at the offset into the array.
template <typename T>
void IndexBuffer::Get(T data[], unsigned int elementCount, unsigned int offsetInBytes)
{
  const unsigned int bytesToRead = sizeof(T) * elementCount;
  
  if (bytesToRead + offsetInBytes <= SizeInBytes)
  {
    gl::BindVertexArray(0);
    gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, buffer);
    gl::GetBufferSubData(gl::ELEMENT_ARRAY_BUFFER, offsetInBytes, bytesToRead, data);
  }
  else
  {
    ASSERT(false && "out-of-bounds on buffer read");
  }
}

//----------------------------------------------------------------

void IndexBuffer::Bind()
{
  gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, buffer);
}

//----------------------------------------------------------------

void IndexBuffer::Unbind()
{
  gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, 0);
}
