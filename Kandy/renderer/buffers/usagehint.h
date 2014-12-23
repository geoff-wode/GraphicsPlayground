#pragma once

namespace Kandy
{
  namespace Renderer
  {
    struct BufferUsageHint
    {
      enum Enum
      {
        StreamDraw,
        StreamRead,
        StreamCopy,
        StaticDraw,
        StaticRead,
        StaticCopy,
        DynamicDraw,
        DynamicRead,
        DynamicCopy
      };
    };
  }
}
