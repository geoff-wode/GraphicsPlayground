#define _USE_MATH_DEFINES
#include <cmath>
#include <cfloat>
#include <set>
#include <string>
#include <sstream>
#include <cstring>
#include <core\logging.h>
#include <scene\scenestate.h>
#include <renderer\shaders\shader.h>
#include <renderer\device.h>
#include <renderer\src\shaders\shadertype.h>
#include <renderer\src\shaders\shaderobject.h>
#include <renderer\shaders\uniforms\alltypes.h>
#include <renderer\src\gl_core_3_3.hpp>
#include <renderer\src\gltypeconversion.h>
#include <boost\foreach.hpp>
#include <boost\thread.hpp>
#include <boost\thread\once.hpp>
#include <boost\shared_ptr.hpp>
#include <boost\make_shared.hpp>
#include <boost\algorithm\string.hpp>

using namespace Kandy::Renderer;
using namespace Kandy::Scene;

//-------------------------------------------------------

struct Shader::Impl
{
  GLuint program;
  Shader::ShaderUniforms uniforms;
  Shader::ShaderVertexAttributes vertexAttributes;
  typedef std::map<std::string, int> ShaderFragmentOutputs;
  ShaderFragmentOutputs fragmentOutputs;

  std::set<ShaderUniformBase*> modifiedUniforms;
};

//-------------------------------------------------------

static void InitialiseBuiltinConstants();
static bool CreateShaderProgram(const char* const vsSrc, const char* const gsSrc, const char* const fsSrc, GLuint& program);
static void FindVertexAttribs(GLuint program, Shader::ShaderVertexAttributes& attrs);
static void FindUniforms(GLuint program, Shader::ShaderUniforms& uniforms, Shader* observer);
static boost::shared_ptr<ShaderUniformBase> NewUniform(const std::string& name, GLenum type, int location, Shader* observer);

//-------------------------------------------------------

static boost::once_flag initOnce = BOOST_ONCE_INIT;
static std::string      builtinConstants;

//-------------------------------------------------------

Shader::Shader()
  : impl(new Impl())
{
  INFO("%p\n", this);
}

//-------------------------------------------------------

Shader::~Shader()
{
  gl::DeleteProgram(impl->program);
  delete impl;
  INFO("%p\n", this);
}

//-------------------------------------------------------

const Shader::ShaderUniforms& Shader::GetUniforms() const
{
  return impl->uniforms;
}

//-------------------------------------------------------

const Shader::ShaderVertexAttributes& Shader::GetVertexAttributes() const
{
  return impl->vertexAttributes;
}

//-------------------------------------------------------

bool Shader::Create(const char* const vertexShader, const char* const fragmentShader)
{
  boost::call_once(InitialiseBuiltinConstants, initOnce);

  const bool created = CreateShaderProgram(vertexShader, NULL, fragmentShader, impl->program);
  if (created)
  {
    FindVertexAttribs(impl->program, impl->vertexAttributes);
    FindUniforms(impl->program, impl->uniforms, this);
    INFO("%p: %d vertexAttributes, %d uniforms, %d fragmentOutputs\n",
      this,
      impl->vertexAttributes.size(),
      impl->uniforms.size(),
      impl->fragmentOutputs.size());
  }
  return created;
}

//-------------------------------------------------------

void Shader::Bind()  // NB. No "unbind"! A shader is always required.
{
  gl::UseProgram(impl->program);
}

//-------------------------------------------------------

void Shader::Update(const RenderState& renderState, const SceneState& sceneState)
{
  // TODO: Update "automatic" uniforms e.g. camera position, etc.

  BOOST_FOREACH(auto uniform, impl->modifiedUniforms)
  {
    uniform->Apply();
  }
  impl->modifiedUniforms.clear();
}

//-------------------------------------------------------

void Shader::NotifyUniformValueChanged(ShaderUniformBase* uniform)
{
  impl->modifiedUniforms.insert(uniform);
}

//-------------------------------------------------------

int Shader::GetFragmentOutput(const std::string& name)
{
  Impl::ShaderFragmentOutputs::iterator pos = impl->fragmentOutputs.find(name);
  if (pos != impl->fragmentOutputs.end())
  {
    return pos->second;
  }
  int location = gl::GetFragDataLocation(impl->program, name.c_str());
  if (location != -1)
  {
    impl->fragmentOutputs[name] = location;
  }
  return location;
}

//-------------------------------------------------------

static bool CreateShaderProgram(const char* const vsSrc, const char* const gsSrc, const char* const fsSrc, GLuint& program)
{
  program = gl::CreateProgram();
  bool created = false;
  std::vector<boost::shared_ptr<ShaderObject>> objects;
  try
  {
    if (vsSrc) { objects.push_back(boost::make_shared<VertexShaderObject>(vsSrc)); }
    if (fsSrc) { objects.push_back(boost::make_shared<FragmentShaderObject>(fsSrc)); }
    if (gsSrc) { objects.push_back(boost::make_shared<GeometryShaderObject>(gsSrc)); }

    BOOST_FOREACH(auto obj, objects)
    {
      gl::AttachShader(program, obj->shader);
    }

    gl::LinkProgram(program);

    GLint okay;
    gl::GetProgramiv(program, gl::LINK_STATUS, &okay);
    created = (0 != okay);
    if (!created)
    {
      GLint logLength;
      gl::GetProgramiv(program, gl::INFO_LOG_LENGTH, &logLength);
      std::vector<GLchar> log(logLength);
      gl::GetProgramInfoLog(program, logLength, NULL, log.data());
      WARNING("cannot link shader:\n%s\n", log.data());
    }
  }
  catch (std::exception& e)
  {
    WARNING("%s\n", e.what());
  }

  BOOST_FOREACH(auto obj, objects)
  {
    gl::DetachShader(program, obj->shader);
  }
  return created;
}

//-------------------------------------------------------

static void FindVertexAttribs(GLuint program, Shader::ShaderVertexAttributes& attrs)
{
  GLint numAttrs;
  gl::GetProgramiv(program, gl::ACTIVE_ATTRIBUTES, &numAttrs);
  GLint maxNameLength;
  gl::GetProgramiv(program, gl::ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameLength);
  std::vector<GLchar> name(maxNameLength);

  for (GLint i = 0; i < numAttrs; ++i)
  {
    GLint nameLength;
    GLint length;
    GLenum type;
    gl::GetActiveAttrib(program, i, maxNameLength, &nameLength, &length, &type, name.data());

    // Ignore reserved names...
    if (std::strcmp("gl_", name.data()) != 0)
    {
      const GLint location = gl::GetAttribLocation(program, name.data());
      ShaderVertexAttribute attr(name.data(), location, length, FromGL(type));
      attrs.insert(std::pair<std::string,ShaderVertexAttribute>(attr.Name, attr));
    }
  }
}

//-------------------------------------------------------

static void FindUniforms(GLuint program, Shader::ShaderUniforms& uniforms, Shader* observer)
{
  GLint uniformCount;
  gl::GetProgramiv(program, gl::ACTIVE_UNIFORMS, &uniformCount);
  GLint maxNameLength;
  gl::GetProgramiv(program, gl::ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);
  std::vector<GLchar> nameBuffer(maxNameLength+1);
  for (int i = 0; i < uniformCount; ++i)
  {
    GLint size;
    GLint nameLength;
    GLenum type;
    gl::GetActiveUniform(program, i, maxNameLength, &nameLength, &size, &type, nameBuffer.data());

    // Skip uniforms in named blocks... (TODO: implement blocks?)
    GLint blockIndex;
    gl::GetActiveUniformsiv(program, 1, (GLuint*)&i, gl::UNIFORM_BLOCK_INDEX, &blockIndex);
    if ( blockIndex == -1)
    {
      // On ATI, array uniforms have a [0] suffix which needs removing...
      if (boost::algorithm::ends_with(nameBuffer, "[0]"))
      {
        nameLength -= 3;
      }
      const std::string name(nameBuffer.data(), nameLength);

      const GLint location = gl::GetUniformLocation(program, name.c_str());
      uniforms[name] = NewUniform(name, type, location, observer);
    }
  }
}

//-------------------------------------------------------

static boost::shared_ptr<ShaderUniformBase> NewUniform(
  const std::string& name, GLenum type, int location, Shader* observer)
{
  switch (type)
  {
  case gl::INT:        return boost::make_shared<ShaderUniformInt>(name, location, observer); break;
  case gl::INT_VEC2:   return boost::make_shared<ShaderUniformIntVector2>(name, location, observer); break;
  case gl::INT_VEC3:   return boost::make_shared<ShaderUniformIntVector3>(name, location, observer); break;
  case gl::INT_VEC4:   return boost::make_shared<ShaderUniformIntVector4>(name, location, observer); break;
  case gl::FLOAT:      return boost::make_shared<ShaderUniformFloat>(name, location, observer); break;
  case gl::FLOAT_VEC2: return boost::make_shared<ShaderUniformFloatVector2>(name, location, observer); break;
  case gl::FLOAT_VEC3: return boost::make_shared<ShaderUniformFloatVector3>(name, location, observer); break;
  case gl::FLOAT_VEC4: return boost::make_shared<ShaderUniformFloatVector4>(name, location, observer); break;
  case gl::FLOAT_MAT2: return boost::make_shared<ShaderUniformFloatMatrix2>(name, location, observer); break;
  case gl::FLOAT_MAT3: return boost::make_shared<ShaderUniformFloatMatrix3>(name, location, observer); break;
  case gl::FLOAT_MAT4: return boost::make_shared<ShaderUniformFloatMatrix4>(name, location, observer); break;
  default: ASSERT(false);
  }
  return nullptr;
}

//-------------------------------------------------------

static void InitialiseBuiltinConstants()
{
  struct VertexAttribLocation
  {
    enum Enum
    {
      Position      = 0,
      PositionLow   = 0,
      PositionHigh,
      Normal,
      Colour,
      TextureCoordStart
    };
  };

  std::stringstream s;
  s << "#version 330\n";

    // Pre-ordained vertex attribute locations...
  s << "#define VertexPosition      layout (location = " << VertexAttribLocation::Position << ") in\n"
    << "#define VertexPositionLow   layout (location = " << VertexAttribLocation::PositionLow << ") in\n"
    << "#define VertexPositionHigh  layout (location = " << VertexAttribLocation::PositionHigh << ") in\n"
    << "#define VertexNormal        layout (location = " << VertexAttribLocation::Normal << ") in\n"
    << "#define VertexColour        layout (location = " << VertexAttribLocation::Colour << ") in\n";
  for (unsigned int i = 0; i < Device::MaxTextureUnits(); ++i)
  {
    s << "#define VertexTextureCoord" << i << "  layout(location = " << i + VertexAttribLocation::TextureCoordStart << ") in\n";
  }

    // Mathematical constants of note...
  s << "const float E         = " << (float)M_E << "f;\n"
    << "const float Pi        = " << (float)M_PI << "f;\n"
    << "const float twoPi     = " << (float)(2 * M_PI) << "f;\n"
    << "const float OneOverPi = " << (float)(1 / M_PI) << "f;\n"
    << "const float piOver2   = " << (float)(M_PI/2) << "f;\n"
    << "const float piOver3   = " << (float)(M_PI/3) << "f;\n"
    << "const float piOver4   = " << (float)(M_PI/4) << "f;\n"
    << "const float piOver6   = " << (float)(M_PI/6) << "f;\n"
    << "const float threePiOver2 = " << (float)((3 * M_PI) / 2) << "f;\n"
    << "const float oneOver2Pi = " << (float)(1 / (2 * M_PI)) << "f;\n"
    << "const float sqrt2         = " << (float)M_SQRT2 << "f;\n"
    << "const float oneOverSqrt2  = " << (float)(1 / M_SQRT2) << "f;\n"
    << "const float minFloat  = " << FLT_MIN << "f;\n"
    << "const float maxFloat  = " << FLT_MAX << "f;\n"
    << "const float epsilon   = " << FLT_EPSILON * 10.0f << "f;\n" // note we multiply the true value by 10 == wiggle room
    ;

  builtinConstants = s.str();
}
