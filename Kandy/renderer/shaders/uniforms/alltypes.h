#pragma once

#include <glm\glm.hpp>
#include <Kandy\renderer\shaders\uniforms\shaderuniform.h>

#define SHADER_UNIFORM_DECL(class_name, data_type) \
  class ShaderUniform##class_name : public ShaderUniform<data_type> \
  { \
  public: \
    ShaderUniform##class_name(const std::string& name, unsigned int location, ShaderUniformObserver* observer) \
      : ShaderUniform(name, location, observer) \
    { \
    } \
  private: \
    void SetGPUValue() const; \
  }

namespace Kandy
{
  namespace Renderer
  {
    SHADER_UNIFORM_DECL(Int, int);
    SHADER_UNIFORM_DECL(IntVector2, glm::ivec2);
    SHADER_UNIFORM_DECL(IntVector3, glm::ivec3);
    SHADER_UNIFORM_DECL(IntVector4, glm::ivec4);
    SHADER_UNIFORM_DECL(Float, float);
    SHADER_UNIFORM_DECL(FloatVector2, glm::vec2);
    SHADER_UNIFORM_DECL(FloatVector3, glm::vec3);
    SHADER_UNIFORM_DECL(FloatVector4, glm::vec4);
    SHADER_UNIFORM_DECL(FloatMatrix2, glm::mat2);
    SHADER_UNIFORM_DECL(FloatMatrix3, glm::mat3);
    SHADER_UNIFORM_DECL(FloatMatrix4, glm::mat4);
  }
}
