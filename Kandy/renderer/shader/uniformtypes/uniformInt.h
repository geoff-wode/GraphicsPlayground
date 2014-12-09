#pragma once

#include <Kandy\renderer\shader\uniform.h>

namespace Kandy
{
  namespace Renderer
  {
    class UniformInt : public UniformBase<int>
    {
    public:
      UniformInt(IUniformObserver* observer, const std::string& name, unsigned int location)
        : UniformBase<int>(observer, name, Uniform::Type::Int, location)
      {
      }
    };
  }
}
