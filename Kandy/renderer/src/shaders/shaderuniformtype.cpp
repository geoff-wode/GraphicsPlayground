/**
  @file shaderuniformtype.cpp
  @brief Defines the implementation of the @see Apply method for each supported @see ShaderUniform type.
  */

#include <glm\gtc\type_ptr.hpp>
#include <Kandy\renderer\shaders\shaderuniformtype.h>
#include "..\gl_core_3_3.hpp"

using namespace Kandy::Renderer;

//----------------------------------------------------------------

void ShaderUniformInt::Apply()        { gl::Uniform1i(Location, value); }
void ShaderUniformIntVector2::Apply() { gl::Uniform2i(Location, value.x, value.y); }
void ShaderUniformIntVector3::Apply() { gl::Uniform3i(Location, value.x, value.y, value.z); }
void ShaderUniformIntVector4::Apply() { gl::Uniform4i(Location, value.x, value.y, value.z, value.w); }

void ShaderUniformFloat::Apply()        { gl::Uniform1f(Location, value); }
void ShaderUniformFloatVector2::Apply() { gl::Uniform2f(Location, value.x, value.y); }
void ShaderUniformFloatVector3::Apply() { gl::Uniform3f(Location, value.x, value.y, value.z); }
void ShaderUniformFloatVector4::Apply() { gl::Uniform4f(Location, value.x, value.y, value.z, value.w); }

void ShaderUniformMatrix2::Apply()  { gl::UniformMatrix2fv(Location, 1, gl::FALSE_, glm::value_ptr(value)); }
void ShaderUniformMatrix3::Apply()  { gl::UniformMatrix3fv(Location, 1, gl::FALSE_, glm::value_ptr(value)); }
void ShaderUniformMatrix4::Apply()  { gl::UniformMatrix4fv(Location, 1, gl::FALSE_, glm::value_ptr(value)); }
