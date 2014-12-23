#include <cstdint>
#include <core\logging.h>
#include <renderer\buffers\vertexbuffer.h>
#include <renderer\src\gl_core_3_3.hpp>
#include <renderer\src\gltypeconversion.h>

using namespace Kandy::Core;
using namespace Kandy::Renderer;

//-------------------------------------------------------

struct VertexBuffer::Impl
{
  GLuint  id;
  GLenum  glUsageHint;
  size_t  sizeInBytes;
  BufferUsageHint::Enum usageHint;
};

//-------------------------------------------------------

VertexBuffer::VertexBuffer(size_t sizeInBytes, BufferUsageHint::Enum usageHint)
  : impl(new Impl())
{
  impl->glUsageHint = ToGL(usageHint);

  gl::GenBuffers(1, &impl->id);

  Bind();
  gl::BindVertexArray(0);
  gl::BufferData(gl::ARRAY_BUFFER, sizeInBytes, NULL, impl->glUsageHint);

  INFO("%p: size=%d\n", this, sizeInBytes);
}

//-------------------------------------------------------

VertexBuffer::~VertexBuffer()
{
  gl::DeleteBuffers(1, &impl->id);
  delete impl;
  INFO("%p: destroyed\n", this);
}

//-------------------------------------------------------

BufferUsageHint::Enum VertexBuffer::UsageHint() const { return impl->usageHint; }

//-------------------------------------------------------

size_t VertexBuffer::SizeInBytes() const { return impl->sizeInBytes; }

//-------------------------------------------------------

void VertexBuffer::Bind() { gl::BindBuffer(gl::ARRAY_BUFFER, impl->id); }

//-------------------------------------------------------

void VertexBuffer::Unbind() { gl::BindBuffer(gl::ARRAY_BUFFER, 0); }

//-------------------------------------------------------

void VertexBuffer::SetData(const void* const source, size_t sizeInBytes, size_t offset)
{
  if ((sizeInBytes + offset) <= impl->sizeInBytes)
  {
    gl::BindVertexArray(0);
    Bind();
    gl::BufferSubData(gl::ARRAY_BUFFER, offset, sizeInBytes, source);
  }
  else
  {
    ASSERT(false && "size exceeds buffer limits\n");
  }
}

//-------------------------------------------------------

void VertexBuffer::GetData(void* const destination, size_t sizeInBytes, size_t offset)
{
  if ((sizeInBytes + offset) <= impl->sizeInBytes)
  {
    gl::BindVertexArray(0);
    Bind();
    gl::BufferSubData(gl::ARRAY_BUFFER, offset, sizeInBytes, destination);
  }
  else
  {
    ASSERT(false && "size exceeds buffer limits\n");
  }
}

//-------------------------------------------------------
