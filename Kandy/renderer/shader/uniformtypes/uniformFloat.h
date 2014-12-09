#pragma once

#include <glm\glm.hpp>
#include <Kandy\renderer\shader\uniform.h>

namespace Kandy
{
  namespace Renderer
  {
    class UniformFloat : public UniformBase<float>
    {
    public:
      UniformFloat(IUniformObserver* observer, const std::string& name, unsigned int location)
        : UniformBase(observer, name, Uniform::Type::Float, location)
      {
      }
    };
  }
}
