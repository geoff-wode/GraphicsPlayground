#include <cstdint>
#include <core\logging.h>
#include <renderer\buffers\indexbuffer.h>
#include <renderer\src\gl_core_3_3.hpp>
#include <renderer\src\gltypeconversion.h>

using namespace Kandy::Core;
using namespace Kandy::Renderer;

//-------------------------------------------------------

struct IndexBuffer::Impl
{
  GLuint  id;
  GLenum  glUsageHint;
  size_t  capacity;
  size_t  dataTypeSize;
  size_t  sizeInBytes;
  IndexBuffer::DataType::Enum dataType;
  BufferUsageHint::Enum usageHint;
};

//-------------------------------------------------------

static size_t SizeOfType(IndexBuffer::DataType::Enum type)
{
  switch (type)
  {
  case IndexBuffer::DataType::UInt16: return sizeof(uint16_t); break;
  case IndexBuffer::DataType::UInt32: return sizeof(uint32_t); break;
  default: ASSERT(false); break;
  }
  return 0;
}

//-------------------------------------------------------

IndexBuffer::IndexBuffer(DataType::Enum type, size_t capacity, BufferUsageHint::Enum usageHint)
  : impl(new Impl())
{
  impl->capacity = capacity;
  impl->dataType = type;
  impl->dataTypeSize = SizeOfType(type);
  impl->glUsageHint = ToGL(usageHint);
  impl->sizeInBytes = capacity * impl->dataTypeSize;
  impl->usageHint = usageHint;

  gl::GenBuffers(1, &impl->id);
  Bind();
  gl::BindVertexArray(0);
  gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, impl->sizeInBytes, NULL, impl->glUsageHint);

  INFO("%p (%d bytes)\n", this, impl->sizeInBytes);
}

//-------------------------------------------------------

IndexBuffer::~IndexBuffer()
{
  gl::DeleteBuffers(1, &impl->id);
  delete impl;
  INFO("%p destroyed\n", this);
}

//-------------------------------------------------------

size_t IndexBuffer::SizeInBytes() const { return impl->sizeInBytes; }

//-------------------------------------------------------

void IndexBuffer::Bind() { gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, impl->id); }

//-------------------------------------------------------

void IndexBuffer::Unbind() { gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, 0); }

//-------------------------------------------------------

void IndexBuffer::SetData(const void* const source, size_t count, size_t offset)
{
  const size_t sizeToCopy = count * impl->dataTypeSize;

  if ((sizeToCopy + offset) <= impl->sizeInBytes)
  {
    gl::BindVertexArray(0);
    Bind();
    gl::BufferSubData(gl::ELEMENT_ARRAY_BUFFER, offset, sizeToCopy, source);
  }
  else
  {
    ASSERT(false && "size exceeds buffer limits\n");
  }
}

//-------------------------------------------------------

void IndexBuffer::GetData(void* const destination, size_t count, size_t offset)
{
  const size_t sizeToCopy = count * impl->dataTypeSize;

  if ((sizeToCopy + offset) <= SizeInBytes())
  {
    gl::BindVertexArray(0);
    Bind();
    gl::GetBufferSubData(gl::ELEMENT_ARRAY_BUFFER, offset, sizeToCopy, destination);
  }
  else
  {
    ASSERT(false && "size exceeds buffer limits\n");
  }
}

//-------------------------------------------------------
