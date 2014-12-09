#pragma once

#include <glm\glm.hpp>
#include <Kandy\renderer\shader\uniform.h>

namespace Kandy
{
  namespace Renderer
  {
    class UniformFloatVector3 : public UniformBase<glm::vec3>
    {
    public:
      UniformFloatVector3(IUniformObserver* observer, const std::string& name, unsigned int location)
        : UniformBase(observer, name, Uniform::Type::FloatVector3, location)
      {
      }
    };
  }
}
