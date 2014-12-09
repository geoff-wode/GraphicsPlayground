#pragma once

#include "..\gl_core_3_3.hpp"
#include <renderer\buffers\usagehint.h>

namespace Kandy
{
  namespace Renderer
  {
    GLenum GLBufferUsage(BufferUsageHint::Enum usage);
  }
}
