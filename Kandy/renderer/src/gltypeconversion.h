#pragma once

// Provide conversions from Kandy enumerations/values to GL ones.

#include "gl_core_3_3.hpp"
#include <renderer\clearstate.h>
#include <renderer\renderstate.h>

namespace Kandy
{
  namespace Renderer
  {
    GLbitfield ToGL(ClearState::Buffers::Enum buffers);
    GLenum ToGL(RasterisationMode::Enum mode);
    GLenum ToGL(DepthTest::Function::Enum fn);
    GLenum ToGL(Face::Enum face);
    GLenum ToGL(WindingOrder::Enum order);
    GLenum ToGL(IndexBuffer::DataType::Enum type);
    GLenum ToGL(BufferUsageHint::Enum hint);
    GLenum ToGL(ComponentType::Enum type);
  }
}
