#pragma once

namespace Kandy
{
  namespace Renderer
  {
    struct BufferUsageHint
    {
      enum Enum
      {
        Static,   // data copied to GPU once and used many times
        Stream,   // data copied to GPU once and used a "few" times
        Dynamic   // data copied to GPU repeatedly (e.g. every frame)
      };
    };
  }
}
