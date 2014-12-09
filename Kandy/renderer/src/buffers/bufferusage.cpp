#include "bufferusage.h"
#include <core\logging.h>

GLenum Kandy::Renderer::GLBufferUsage(BufferUsageHint::Enum usage)
{
  switch (usage)
  {
  case BufferUsageHint::Static:  return gl::STATIC_DRAW; break;
  case BufferUsageHint::Stream:  return gl::STREAM_DRAW; break;
  case BufferUsageHint::Dynamic: return gl::DYNAMIC_DRAW; break;
  default: ASSERT(false); break;
  }
  return (GLenum)0;
}
