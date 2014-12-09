#pragma once

#include <glm\glm.hpp>
#include <Kandy\renderer\shader\uniform.h>

namespace Kandy
{
  namespace Renderer
  {
    class UniformFloatVector2 : public UniformBase<glm::vec2>
    {
    public:
      UniformFloatVector2(IUniformObserver* observer, const std::string& name, unsigned int location)
        : UniformBase(observer, name, Uniform::Type::FloatVector2, location)
      {
      }
    };
  }
}
