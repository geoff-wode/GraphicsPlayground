#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <sstream>
#include <vector>
#include <boost\make_shared.hpp>
#include <boost\foreach.hpp>

#include <core\logging.h>
#include <renderer\device.h>
#include <renderer\shader.h>
#include <renderer\shader\uniformtypes\all.h>
#include <renderer\shader\vertexattribute.h>

#include <renderer\src\gl_core_3_3.hpp>

using namespace Kandy;
using namespace Kandy::Renderer;

//--------------------------------------------------------------

static const char* const GetConstants();
static GLuint CompileShader(const GLenum type, const char* const src);
static Uniform::Ptr CreateUniform(Shader* shader, const std::string& name, const GLint location, const GLenum type);

//--------------------------------------------------------------

Shader::Ptr Shader::Create(const char* const vsSrc, const char* const fsSrc)
{
  Ptr shader(new Shader());
  const GLuint vs = CompileShader(gl::VERTEX_SHADER, vsSrc);
  const GLuint fs = CompileShader(gl::FRAGMENT_SHADER, fsSrc);

  if (vs && fs)
  {
    gl::AttachShader(shader->program, vs);
    gl::AttachShader(shader->program, fs);
    gl::LinkProgram(shader->program);
    GLint linked;
    gl::GetProgramiv(shader->program, gl::LINK_STATUS, &linked);
    if (linked)
    {
      if (!shader->GetUniforms())
      {
        shader.reset();
      }
    }
    else
    {
      GLint length;
      gl::GetProgramiv(shader->program, gl::INFO_LOG_LENGTH, &length);
      std::vector<GLchar> info(length);
      gl::GetProgramInfoLog(shader->program, length, NULL, info.data());
      LOG("shader link error:\n%s\n");
      shader.reset();
    }
    gl::DetachShader(shader->program, vs);
    gl::DetachShader(shader->program, fs);
  }
  else
  {
    shader.reset();
  }

  gl::DeleteShader(vs);
  gl::DeleteShader(fs);

  return shader;
}

//--------------------------------------------------------------

Shader::Shader()
  : Uniforms(uniforms),
    program(gl::CreateProgram())
{
}

//--------------------------------------------------------------

Shader::~Shader()
{
  gl::DeleteProgram(program);
}

//--------------------------------------------------------------

void Shader::Activate()
{
  gl::UseProgram(program);
}

//--------------------------------------------------------------

void Shader::UpdateUniforms()
{
  for (size_t i = 0; i < changedUniforms.size(); ++i)
  {
    changedUniforms[i]->Update();
  }
  changedUniforms.clear();
}

//--------------------------------------------------------------

void Shader::HasChanged(Uniform* uniform)
{
  changedUniforms.push_back(uniform);
}

//--------------------------------------------------------------

bool Shader::GetUniforms()
{
  GLint uniformCount;
  gl::GetProgramiv(program, gl::ACTIVE_UNIFORMS, &uniformCount);
  GLint uniformMaxNameLength;
  gl::GetProgramiv(program, gl::ACTIVE_UNIFORM_MAX_LENGTH, &uniformMaxNameLength);
  std::vector<GLchar> nameBuffer(uniformMaxNameLength + 1);

  for (int i = 0; i < uniformCount; ++i)
  {
    GLint nameLength;
    GLint size;
    GLenum type;
    gl::GetActiveUniform(program, i, uniformMaxNameLength, &nameLength, &size, &type, nameBuffer.data());

    if (1 != size)
    {
      LOG("WARNING: uniform arrays not supported!\n");
      return false;
    }

    // Ignore reserved gl names...
    if (0 != strcmp("gl_", nameBuffer.data()))
    {
      std::string uniformName(nameBuffer.begin(), nameBuffer.end());
      // Fix up ATI array names (they're suffixed with "[0]")...
      if (std::string::npos != uniformName.find_last_of("[0]"))
      {
        uniformName.resize(uniformName.length() - 3);
      }
      // Ignore uniforms in blocks...
      GLint blockIndex;
      gl::GetActiveUniformsiv(program, 1, (const GLuint*)&i, gl::UNIFORM_BLOCK_INDEX, &blockIndex);
      if (-1 == blockIndex)
      {
        // Finally..!
        const GLint location = gl::GetUniformLocation(program, uniformName.c_str());
        Uniform::Ptr u = CreateUniform(this, uniformName, location, type);
        if (u)
        {
          uniforms[uniformName] = u;
        }
      }
    }
  }

  // Pre-allocate space to hold uniforms that change value at runtime (they're only pointers so
  // it's not _that_ wasteful...)
  changedUniforms.reserve(uniformCount);

  // Hook in any automatic uniforms...
  BOOST_FOREACH(UniformSetPair u, uniforms)
  {
    // Note that ".at()" below will throw if the key is not found. I _hate_ exception
    // programming but this wraps up the search and retrieval into a single call, so I'm
    // being pragmatic (but still need a shower)...
    try
    {
      AutoUniform::Factory::Ptr factory = Device::GetAutoUniformFactories().at(u.first);
      automaticUniforms.push_back(factory->Create(u.second));
    }
    catch(const std::out_of_range& e)
    {
      (void)e; // do nothing
    }
  }

  return true;
}

//--------------------------------------------------------------

void Shader::UpdateAutoUniforms(const PipelineState& pipelineState, const Scene::SceneState& sceneState)
{
  for (size_t i = 0; i < automaticUniforms.size(); ++i)
  {
    automaticUniforms[i]->Set(pipelineState, sceneState);
  }
}

//--------------------------------------------------------------

static Uniform::Ptr CreateUniform(Shader* shader, const std::string& name, const GLint location, const GLenum type)
{
  switch (type)
  {
  case gl::INT:         return boost::make_shared<UniformInt>(shader, name, location); break;
  case gl::FLOAT:       return boost::make_shared<UniformFloat>(shader, name, location); break;
  case gl::FLOAT_VEC2:  return boost::make_shared<UniformFloatVector2>(shader, name, location); break;
  case gl::FLOAT_VEC3:  return boost::make_shared<UniformFloatVector3>(shader, name, location); break;
  case gl::FLOAT_VEC4:  return boost::make_shared<UniformFloatVector4>(shader, name, location); break;
  case gl::FLOAT_MAT4:  return boost::make_shared<UniformFloatMatrix4x4>(shader, name, location); break;
  default: break;
  }

  return NULL;
}

//--------------------------------------------------------------

static GLuint CompileShader(const GLenum type, const char* const src)
{
  std::string modifiedSrc;
  if (0 == (strcmp("#version", src)))
  {
    modifiedSrc = "//";
    modifiedSrc.append(src);
  }
  else
  {
    modifiedSrc.assign(src);
  }
  const char* const sources[] = { GetConstants(), modifiedSrc.c_str() };
  const int lengths[] = { strlen(GetConstants()), modifiedSrc.length() };
  const size_t sourcesCount = sizeof(sources) / sizeof(sources[0]);

  const GLuint shader = gl::CreateShader(type);
  if (shader)
  {
    gl::ShaderSource(shader, sourcesCount, sources, lengths);
    gl::CompileShader(shader);
    GLint compiled;
    gl::GetShaderiv(shader, gl::COMPILE_STATUS, &compiled);
    if (!compiled)
    {
      GLint length;
      gl::GetShaderiv(shader, gl::INFO_LOG_LENGTH, &length);
      std::vector<GLchar> info(length);
      gl::GetShaderInfoLog(shader, length, NULL, info.data());
      LOG("shader compilation error\n:%s\n");
    }
  }

  return shader;
}

//--------------------------------------------------------------

static const char* const GetConstants()
{
  static bool defined = false;
  static std::string constants;
  if (!defined)
  {
    std::stringstream s;
    s << "#version 330\n"
      // Standard vertex attribute locations:
      << "#define k_vertexPositionLocation        layout ( location = " << ShaderVertexAttributeLocation::Position << " )\n"
      << "#define k_vertexPositionHighLocation    layout ( location = " << ShaderVertexAttributeLocation::PositionHigh << " )\n"
      << "#define k_vertexPositionLowLocation     layout ( location = " << ShaderVertexAttributeLocation::PositionLow << " )\n"
      << "#define k_vertexNormalLocation          layout ( location = " << ShaderVertexAttributeLocation::Normal << " )\n"
      << "#define k_vertexTextCoordinateLocation  layout ( location = " << ShaderVertexAttributeLocation::TextureCoordinate << " )\n"
      << "#define k_vertexColourLocation          layout ( location = " << ShaderVertexAttributeLocation::Colour<< " )\n"
      // Various mathematical constants:
      << "const float k_pi      = " << (float)M_PI << "\n"
      << "const float k_2pi     = " << (float)(2 * M_PI) << "\n"
      << "const float k_piOver2 = " << (float)(M_PI / 2) << "\n"
      ;
    constants = s.str();
    defined = true;
    LOG("GLSL constants:\n%s\n", constants.c_str());
  }
  return constants.c_str();
}
