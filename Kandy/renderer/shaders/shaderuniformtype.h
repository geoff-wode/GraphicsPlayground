#pragma once

#include <glm\glm.hpp>
#include <Kandy\renderer\shaders\shaderuniformgeneric.h>

/**
  @file shaderuniformtype.h

  Declares type-specific instances of @see ShaderUniform.
  */

/**
  Helper macro to generate the boilerplate interface to the strongly-type @see ShaderUniform
  instances.
  */
#if !defined(KANDY_DECLARE_SHADER_UNIFORM)
#define KANDY_DECLARE_SHADER_UNIFORM(typeName, type) \
  struct ShaderUniform##typeName : public ShaderUniformGeneric<type> \
  { \
    ShaderUniform##typeName(unsigned int location, const char* const name, ShaderUniformObserver* const observer) \
      : ShaderUniformGeneric(location, name, observer) \
      { \
      } \
    virtual void Apply(); \
  }
#endif

namespace Kandy
{
  namespace Renderer
  {
    KANDY_DECLARE_SHADER_UNIFORM(Int, int);
    KANDY_DECLARE_SHADER_UNIFORM(IntVector2, glm::ivec2);
    KANDY_DECLARE_SHADER_UNIFORM(IntVector3, glm::ivec3);
    KANDY_DECLARE_SHADER_UNIFORM(IntVector4, glm::ivec4);

    KANDY_DECLARE_SHADER_UNIFORM(Float, float);
    KANDY_DECLARE_SHADER_UNIFORM(FloatVector2, glm::vec2);
    KANDY_DECLARE_SHADER_UNIFORM(FloatVector3, glm::vec3);
    KANDY_DECLARE_SHADER_UNIFORM(FloatVector4, glm::vec4);

    KANDY_DECLARE_SHADER_UNIFORM(Matrix2, glm::mat2);
    KANDY_DECLARE_SHADER_UNIFORM(Matrix3, glm::mat3);
    KANDY_DECLARE_SHADER_UNIFORM(Matrix4, glm::mat4);
  }
}
