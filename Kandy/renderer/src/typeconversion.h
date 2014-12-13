#pragma once

#include <core\logging.h>
#include <renderer\shader.h>
#include <renderer\buffers\usagehint.h>
#include "gl_core_3_3.hpp"

namespace Kandy
{
  namespace Renderer
  {
    GLenum ConvertUniformType(Uniform::Type::Enum type)
    {
      switch (type)
      {
      case Uniform::Type::Int:            return gl::INT; break;
      case Uniform::Type::Float:          return gl::FLOAT; break;
      case Uniform::Type::FloatVector2:   return gl::FLOAT_VEC2; break;
      case Uniform::Type::FloatVector3:   return gl::FLOAT_VEC3; break;
      case Uniform::Type::FloatVector4:   return gl::FLOAT_VEC4; break;
      case Uniform::Type::FloatMatrix4x4: return gl::FLOAT_MAT4; break;
      default: ASSERT(false);
      }
      return (GLenum)0;
    }

    //------------------------------------------------------

    Uniform::Type::Enum ConvertUniformType(GLenum type)
    {
      switch (type)
      {
      case gl::INT:        return Uniform::Type::Int; break;
      case gl::FLOAT:      return Uniform::Type::Float; break;
      case gl::FLOAT_VEC2: return Uniform::Type::FloatVector2; break;
      case gl::FLOAT_VEC3: return Uniform::Type::FloatVector3; break;
      case gl::FLOAT_VEC4: return Uniform::Type::FloatVector4; break;
      case gl::FLOAT_MAT4: return Uniform::Type::FloatMatrix4x4; break;
      default: ASSERT(false);
      }
      return Uniform::Type::None;
    }

    //--------------------------------------------------------------

    static ShaderVertexAttributeType::Enum ConvertShaderVertexAttributeType(GLenum type)
    {
      switch (type)
      {
      case gl::INT:         return ShaderVertexAttributeType::Int; break;
      case gl::FLOAT:       return ShaderVertexAttributeType::Float; break;
      case gl::FLOAT_VEC2:  return ShaderVertexAttributeType::FloatVector2; break;
      case gl::FLOAT_VEC3:  return ShaderVertexAttributeType::FloatVector3; break;
      case gl::FLOAT_VEC4:  return ShaderVertexAttributeType::FloatVector4; break;
      case gl::FLOAT_MAT4:  return ShaderVertexAttributeType::FloatMatrix4x4; break;
      default: ASSERT(false);
      }
      return ShaderVertexAttributeType::None;
    }
  }
}
