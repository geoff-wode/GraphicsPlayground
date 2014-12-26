#pragma once

// Provide conversions from Kandy enumerations/values to GL ones.

#include <core\logging.h>
#include <renderer\src\gltypeconversion.h>

namespace Kandy
{
  namespace Renderer
  {
    GLbitfield ToGL(ClearState::Buffers::Enum buffers)
    {
      GLbitfield bits = 0;
      if (buffers & ClearState::Buffers::Colour)  { bits |= gl::COLOR_BUFFER_BIT; }
      if (buffers & ClearState::Buffers::Depth)   { bits |= gl::DEPTH_BUFFER_BIT; }
      if (buffers & ClearState::Buffers::Stencil) { bits |= gl::STENCIL_BUFFER_BIT; }
      return bits;
    }

    //-------------------------------------------------------------------

    GLenum ToGL(RasterisationMode::Enum mode)
    {
      switch (mode)
      {
      case RasterisationMode::Point:  return gl::POINT; break;
      case RasterisationMode::Line:   return gl::LINE; break;
      case RasterisationMode::Fill:   return gl::FILL; break;
      default: break;
      }
      ASSERT(false);
      return 0;
    }

    //-------------------------------------------------------------------

    GLenum ToGL(DepthTest::Function::Enum fn)
    {
      switch (fn)
      {
      case DepthTest::Function::Never:              return gl::NEVER; break;
      case DepthTest::Function::Less:               return gl::LESS; break;
      case DepthTest::Function::Equal:              return gl::EQUAL; break;
      case DepthTest::Function::LessThanOrEqual:    return gl::LEQUAL; break;
      case DepthTest::Function::Greater:            return gl::GREATER; break;
      case DepthTest::Function::NotEqual:           return gl::NOTEQUAL; break;
      case DepthTest::Function::GreaterThanOrEqual: return gl::GEQUAL; break;
      case DepthTest::Function::Always:             return gl::ALWAYS; break;
      default: break;
      }
      ASSERT(false);
      return 0;
    }

    //-------------------------------------------------------------------

    GLenum ToGL(Face::Enum face)
    {
      switch (face)
      {
      case Face::Front:         return gl::FRONT; break;
      case Face::Back:          return gl::BACK; break;
      case Face::FrontAndBack:  return gl::FRONT_AND_BACK; break;
      default: break;
      }
      ASSERT(false);
      return 0;
    }

    //-------------------------------------------------------------------

    GLenum ToGL(WindingOrder::Enum order)
    {
      switch (order)
      {
      case WindingOrder::Clockwise:         return gl::CW; break;
      case WindingOrder::CounterClockwise:  return gl::CCW; break;
      default: break;
      }
      ASSERT(false);
      return 0;
    }

    //-------------------------------------------------------------------

    GLenum ToGL(IndexBuffer::DataType::Enum type)
    {
      switch (type)
      {
      case IndexBuffer::DataType::UInt16: return gl::UNSIGNED_SHORT; break;
      case IndexBuffer::DataType::UInt32: return gl::UNSIGNED_INT; break;
      default: break;
      }
      ASSERT(false);
      return 0;
    }

    //-------------------------------------------------------------------

    GLenum ToGL(BufferUsageHint::Enum hint)
    {
      switch (hint)
      {
      case BufferUsageHint::StreamDraw:   return gl::STREAM_DRAW; break;
      case BufferUsageHint::StreamRead:   return gl::STREAM_READ; break;
      case BufferUsageHint::StreamCopy:   return gl::STREAM_COPY; break;
      case BufferUsageHint::StaticDraw:   return gl::STATIC_DRAW; break;
      case BufferUsageHint::StaticRead:   return gl::STATIC_READ; break;
      case BufferUsageHint::StaticCopy:   return gl::STATIC_COPY; break;
      case BufferUsageHint::DynamicDraw:  return gl::DYNAMIC_DRAW; break;
      case BufferUsageHint::DynamicRead:  return gl::DYNAMIC_READ; break;
      case BufferUsageHint::DynamicCopy:  return gl::DYNAMIC_COPY; break;
      default: break;
      }
      ASSERT(false);
      return 0;
    }

    //-------------------------------------------------------------------

    GLenum ToGL(ComponentType::Enum type)
    {
      switch (type)
      {
      case ComponentType::Byte:           return gl::BYTE; break;
      case ComponentType::UnsignedByte:   return gl::UNSIGNED_BYTE; break;
      case ComponentType::Short:          return gl::SHORT; break;
      case ComponentType::UnsignedShort:  return gl::UNSIGNED_SHORT; break;
      case ComponentType::Int:            return gl::INT; break;
      case ComponentType::UnsignedInt:    return gl::UNSIGNED_INT; break;
      case ComponentType::Float:          return gl::FLOAT; break;
      case ComponentType::HalfFloat:      return gl::HALF_FLOAT; break;
      default: break;
      }
      ASSERT(false);
      return 0;
    }

    //-------------------------------------------------------------------

    GLenum ToGL(ShaderType::Enum type)
    {
      switch (type)
      {
      case ShaderType::Vertex:    return gl::VERTEX_SHADER; break;
      case ShaderType::Fragment:  return gl::FRAGMENT_SHADER; break;
      case ShaderType::Geometry:  return gl::GEOMETRY_SHADER; break;
      default: break;
      }
      ASSERT(false);
      return 0;
    }

    //-------------------------------------------------------------------

    GLenum ToGL(ShaderVertexAttribType::Enum type)
    {
      switch (type)
      {
      case ShaderVertexAttribType::Float:         return gl::FLOAT ;break;
      case ShaderVertexAttribType::FloatVector2:  return gl::FLOAT_VEC2 ;break;
      case ShaderVertexAttribType::FloatVector3:  return gl::FLOAT_VEC3 ;break;
      case ShaderVertexAttribType::FloatVector4:  return gl::FLOAT_VEC4 ;break;
      case ShaderVertexAttribType::FloatMatrix22: return gl::FLOAT_MAT2 ;break;
      case ShaderVertexAttribType::FloatMatrix33: return gl::FLOAT_MAT3 ;break;
      case ShaderVertexAttribType::FloatMatrix44: return gl::FLOAT_MAT4 ;break;
      case ShaderVertexAttribType::Int:           return gl::INT ;break;
      case ShaderVertexAttribType::IntVector2:    return gl::INT_VEC2 ;break;
      case ShaderVertexAttribType::IntVector3:    return gl::INT_VEC3 ;break;
      case ShaderVertexAttribType::IntVector4:    return gl::INT_VEC4 ;break;
      default: break;
      }
      ASSERT(false);
      return 0;
    }

    //-------------------------------------------------------------------

    ShaderVertexAttribType::Enum FromGL(GLenum type)
    {
      switch (type)
      {
      case gl::FLOAT:       return ShaderVertexAttribType::Float         ; break;
      case gl::FLOAT_VEC2:  return ShaderVertexAttribType::FloatVector2  ; break;
      case gl::FLOAT_VEC3:  return ShaderVertexAttribType::FloatVector3  ; break;
      case gl::FLOAT_VEC4:  return ShaderVertexAttribType::FloatVector4  ; break;
      case gl::FLOAT_MAT2:  return ShaderVertexAttribType::FloatMatrix22 ; break;
      case gl::FLOAT_MAT3:  return ShaderVertexAttribType::FloatMatrix33 ; break;
      case gl::FLOAT_MAT4:  return ShaderVertexAttribType::FloatMatrix44 ; break;
      case gl::INT:         return ShaderVertexAttribType::Int           ; break;
      case gl::INT_VEC2:    return ShaderVertexAttribType::IntVector2    ; break;
      case gl::INT_VEC3:    return ShaderVertexAttribType::IntVector3    ; break;
      case gl::INT_VEC4:    return ShaderVertexAttribType::IntVector4    ; break;
      default: break;
      }
      ASSERT(false);
      return ShaderVertexAttribType::Float;
    }

    //-------------------------------------------------------------------

    GLenum ToGL(ShaderUniformType::Enum type)
    {
      switch (type)
      {
      case ShaderUniformType::Int:           return gl::INT; break;
      case ShaderUniformType::Float:         return gl::FLOAT; break;
      case ShaderUniformType::FloatVector2:  return gl::FLOAT_VEC2; break;
      case ShaderUniformType::FloatVector3:  return gl::FLOAT_VEC3; break;
      case ShaderUniformType::FloatVector4:  return gl::FLOAT_VEC4; break;
      case ShaderUniformType::FloatMatrix22: return gl::FLOAT_MAT2; break;
      case ShaderUniformType::FloatMatrix33: return gl::FLOAT_MAT3; break;
      case ShaderUniformType::FloatMatrix44: return gl::FLOAT_MAT2; break;
      default: break;
      }
      ASSERT(false);
      return 0;
    }
  }
}
