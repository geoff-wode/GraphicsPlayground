#pragma once

#include <glm\glm.hpp>
#include <Kandy\renderer\shader\uniform.h>

namespace Kandy
{
  namespace Renderer
  {
    class UniformFloatVector4 : public UniformBase<glm::vec4>
    {
    public:
      UniformFloatVector4(IUniformObserver* observer, const std::string& name, unsigned int location)
        : UniformBase(observer, name, Uniform::Type::FloatVector4, location)
      {
      }
    };
  }
}
