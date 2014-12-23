#include <core\logging.h>
#include <renderer\device.h>
#include <renderer\vertexarray.h>
#include "gl_core_3_3.hpp"

using namespace Kandy::Renderer;

//---------------------------------------------------------

struct VertexArray::Impl
{
  GLuint id;
  size_t stride;
  std::vector<VertexArrayAttribute> attrs;
};

//---------------------------------------------------------

VertexArray::VertexArray()
  : impl(new Impl())
{
  gl::GenVertexArrays(1, &impl->id);
  INFO("%p\n", this);
}

//---------------------------------------------------------

VertexArray::~VertexArray()
{
  gl::DeleteVertexArrays(1, &impl->id);
  delete impl;
  INFO("%p\n", this);
}

//---------------------------------------------------------

bool VertexArray::AddAttribute(const VertexArrayAttribute& attr)
{
  if (impl->attrs.size() == Device::MaxVertexAttributes())
  {
    return false;
  }
  impl->attrs.push_back(attr);
  impl->stride += attr.SizeInBytes();
  INFO("%p: attr #%d\n", this, impl->attrs.size());
  return true;
}

//---------------------------------------------------------

const std::vector<VertexArrayAttribute>& VertexArray::GetAttributes() const
{
  return impl->attrs;
}

//---------------------------------------------------------

size_t VertexArray::Stride() const
{
  return impl->stride;
}

//---------------------------------------------------------

void VertexArray::Bind() const
{
  gl::BindVertexArray(impl->id);
}

//---------------------------------------------------------

void VertexArray::Unbind()
{
  gl::BindVertexArray(0);
}
