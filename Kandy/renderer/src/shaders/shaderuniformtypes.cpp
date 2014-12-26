#include <glm\gtc\type_ptr.hpp>
#include <renderer\shaders\uniforms\alltypes.h>
#include <renderer\shaders\uniforms\shaderuniformobserver.h>
#include "..\gl_core_3_3.hpp"

using namespace Kandy::Renderer;

//--------------------------------------------------------

void ShaderUniformInt::SetGPUValue() const          { gl::Uniform1i(Location, value); }
void ShaderUniformIntVector2::SetGPUValue() const   { gl::Uniform2i(Location, value.x, value.y); }
void ShaderUniformIntVector3::SetGPUValue() const   { gl::Uniform3i(Location, value.x, value.y, value.z); }
void ShaderUniformIntVector4::SetGPUValue() const   { gl::Uniform4i(Location, value.x, value.y, value.z, value.w); }
void ShaderUniformFloat::SetGPUValue() const        { gl::Uniform1f(Location, value); }
void ShaderUniformFloatVector2::SetGPUValue() const { gl::Uniform2f(Location, value.x, value.y); }
void ShaderUniformFloatVector3::SetGPUValue() const { gl::Uniform3f(Location, value.x, value.y, value.z); }
void ShaderUniformFloatVector4::SetGPUValue() const { gl::Uniform4f(Location, value.x, value.y, value.z, value.w); }
void ShaderUniformFloatMatrix2::SetGPUValue() const { gl::UniformMatrix2fv(Location, 1, gl::FALSE_, glm::value_ptr(value)); }
void ShaderUniformFloatMatrix3::SetGPUValue() const { gl::UniformMatrix3fv(Location, 1, gl::FALSE_, glm::value_ptr(value)); }
void ShaderUniformFloatMatrix4::SetGPUValue() const { gl::UniformMatrix4fv(Location, 1, gl::FALSE_, glm::value_ptr(value)); }
