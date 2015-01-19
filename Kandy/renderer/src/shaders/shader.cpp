#include <exception>
#include <vector>
#include <boost\algorithm\string.hpp>
#include <boost\make_shared.hpp>
#include <Kandy\core\logging.h>
#include <Kandy\renderer\shaders\shaderuniformtype.h>
#include "shaderimpl.h"
#include "shaderobject.h"
#include "..\gl_core_3_3.hpp"

using namespace Kandy::Core;
using namespace Kandy::Renderer;

//--------------------------------------------------

static void LinkShader(const unsigned int handle, std::vector<ShaderObject> objects);
static void FindUniforms(ShaderUniformObserver* observer, const unsigned int handle, Shader::Uniforms& uniforms);
static boost::shared_ptr<ShaderUniform> NewUniform(
    const char* const name,
    GLenum type,
    unsigned int location,
    ShaderUniformObserver* observer);

//--------------------------------------------------

ShaderImpl::ShaderImpl(
  const char* const vertexShaderSource,
  const char* const fragmentShaderSource)
  : handle(gl::CreateProgram())
{
  std::vector<ShaderObject> objects;
  try
  {
    objects.push_back(ShaderObject(ShaderObject::Type::VertexShader, vertexShaderSource));
    objects.push_back(ShaderObject(ShaderObject::Type::FragmentShader, fragmentShaderSource));
    LinkShader(handle, objects);
    FindUniforms(this, handle, uniforms);
    INFO("shader compiled - %d uniforms\n", uniforms.size());
  } catch (const std::logic_error& e)
  {
    throw e;
  }
}

//--------------------------------------------------

ShaderImpl::~ShaderImpl()
{
  gl::DeleteProgram(handle);
}

//--------------------------------------------------

void ShaderImpl::Bind()
{
  gl::UseProgram(handle);
}

//--------------------------------------------------

void ShaderImpl::Modified(const ShaderUniform* const uniform)
{
  changedUniforms.insert(uniform);
}

//--------------------------------------------------

static void LinkShader(const unsigned int handle, std::vector<ShaderObject> objects)
{
  for (size_t i = 0; i < objects.size(); ++i)
  {
    gl::AttachShader(handle, objects[i].handle);
  }
  gl::LinkProgram(handle);
  for (size_t i = 0; i < objects.size(); ++i)
  {
    gl::DetachShader(handle, objects[i].handle);
  }
  GLint linkedOkay;
  gl::GetProgramiv(handle, gl::LINK_STATUS, &linkedOkay);
  if (!linkedOkay)
  {
    GLint logLength;
    gl::GetProgramiv(handle, gl::INFO_LOG_LENGTH, &logLength);
    std::vector<GLchar> log(logLength);
    gl::GetProgramInfoLog(handle, logLength, NULL, log.data());
    WARNING("cannot link shader:\n%s\n", log.data());
    throw new std::logic_error("cannot link shader");
  }
}

//--------------------------------------------------

static void FindUniforms(ShaderUniformObserver* observer, const unsigned int handle, Shader::Uniforms& uniforms)
{
  GLint count;
  gl::GetProgramiv(handle, gl::ACTIVE_UNIFORMS, &count);
  GLint maxNameLength;
  gl::GetProgramiv(handle, gl::ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);
  std::vector<GLchar> nameBuffer(maxNameLength+1);
  for (int i = 0; i < count; ++i)
  {
    GLint size;
    GLint nameLength;
    GLenum type;
    gl::GetActiveUniform(handle, i, maxNameLength, &nameLength, &size, &type, nameBuffer.data());
    // Skip named blocks...
    GLint blockIndex;
    gl::GetActiveUniformsiv(handle, 1, (GLuint*)&i, gl::UNIFORM_BLOCK_INDEX, &blockIndex);
    if (blockIndex == -1)
    {
      // Handle ATI array names which are post-fixed with "[0]"...
      if (boost::algorithm::ends_with(nameBuffer, "[0]")) { nameLength -= 3; }
      const std::string name(nameBuffer.data(), nameLength);
      const GLint location = gl::GetUniformLocation(handle, name.c_str());
      uniforms[name] = NewUniform(name.c_str(), type, location, observer);
    }
  }
}

//--------------------------------------------------

static boost::shared_ptr<ShaderUniform> NewUniform(
  const char* const name,
  GLenum type,
  unsigned int location,
  ShaderUniformObserver* observer)
{
  switch (type)
  {
  case gl::INT:        return boost::make_shared<ShaderUniformInt>(location, name, observer); break;
  case gl::INT_VEC2:   return boost::make_shared<ShaderUniformIntVector2>(location, name, observer); break;
  case gl::INT_VEC3:   return boost::make_shared<ShaderUniformIntVector3>(location, name, observer); break;
  case gl::INT_VEC4:   return boost::make_shared<ShaderUniformIntVector4>(location, name, observer); break;
  case gl::FLOAT:      return boost::make_shared<ShaderUniformFloat>(location, name, observer); break;
  case gl::FLOAT_VEC2: return boost::make_shared<ShaderUniformFloatVector2>(location, name, observer); break;
  case gl::FLOAT_VEC3: return boost::make_shared<ShaderUniformFloatVector3>(location, name, observer); break;
  case gl::FLOAT_VEC4: return boost::make_shared<ShaderUniformFloatVector4>(location, name, observer); break;
  case gl::FLOAT_MAT2: return boost::make_shared<ShaderUniformMatrix2>(location, name, observer); break;
  case gl::FLOAT_MAT3: return boost::make_shared<ShaderUniformMatrix3>(location, name, observer); break;
  case gl::FLOAT_MAT4: return boost::make_shared<ShaderUniformMatrix4>(location, name, observer); break;
  default: ASSERT(false);
  }
  boost::shared_ptr<ShaderUniform> null;
  return null;
}
