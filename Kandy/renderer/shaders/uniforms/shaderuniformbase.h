#pragma once

#include <string>
#include <Kandy\renderer\shaders\uniforms\uniformtypes.h>

namespace Kandy
{
  namespace Renderer
  {
    class ShaderUniformObserver;

    class ShaderUniformBase
    {
    public:
      virtual ~ShaderUniformBase() { }

      const unsigned int Location;
      const std::string Name;

      void Apply()
      {
        SetGPUValue();
      }

    protected:
      ShaderUniformBase(const std::string& name, unsigned int location, ShaderUniformObserver* observer)
        : Location(location),
          Name(name),
          observer(observer)
      {
      }

      ShaderUniformObserver* observer;

    private:
      virtual void SetGPUValue() const = 0;
    };
  }
}
