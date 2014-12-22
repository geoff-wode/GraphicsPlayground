#pragma once

#include <glm\glm.hpp>

namespace Kandy
{
  namespace Renderer
  {
    // Defines the _maximum_ preferred visual attributes of the graphics device.
    // Not all preferences may be met by the underlying hardware and some may
    // exceed those specified if deemed appropriate.
    struct DeviceContext
    {
      DeviceContext()
        : fakeFullScreen(true),
          doubleBuffered(true),
          colourDepth(8,8,8,8)
      { }

      bool fakeFullScreen;  // use the current desktop instead of doing a full video mode change

      bool doubleBuffered;
      
      glm::ivec4 colourDepth;
    };
  }
}
