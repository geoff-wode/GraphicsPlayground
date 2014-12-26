#pragma once

#include <Kandy\renderer\shaders\uniforms\shaderuniformbase.h>

namespace Kandy
{
  namespace Renderer
  {
    class ShaderUniformObserver
    {
    public:
      virtual void NotifyUniformValueChanged(ShaderUniformBase* uniform) = 0;
    };
  }
}
