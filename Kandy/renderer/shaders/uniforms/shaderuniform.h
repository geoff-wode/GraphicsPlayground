#pragma once

#include <Kandy\renderer\shaders\uniforms\shaderuniformbase.h>

namespace Kandy
{
  namespace Renderer
  {
    template <typename T>
    class ShaderUniform : public ShaderUniformBase
    {
    public:
      virtual ~ShaderUniform() { }

      void Set(const T& in)
      {
        if (in != value)
        {
          value = in;
          observer->NotifyUniformValueChanged(this);
        }
      }

      T Get() const { return value; }

    protected:
      ShaderUniform(const std::string& name, unsigned int location, ShaderUniformObserver* observer)
        : ShaderUniformBase(name, location, observer)
      {
      }

      T value;
    };
  }
}
