#pragma once

#include <glm\glm.hpp>
#include <Kandy\renderer\shader\uniform.h>

namespace Kandy
{
  namespace Renderer
  {
    class UniformFloatMatrix4x4 : public UniformBase<glm::mat4>
    {
    public:
      UniformFloatMatrix4x4(IUniformObserver* observer, const std::string& name, unsigned int location)
        : UniformBase(observer, name, Uniform::Type::FloatMatrix4x4, location)
      {
      }
    };
  }
}
