#pragma once

namespace Kandy
{
  namespace Renderer
  {
    class Uniform;

    struct IUniformObserver
    {
      virtual void HasChanged(Uniform* uniform) = 0;
    };
  }
}
