#include <exception>
#include <sstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <float.h>

#include <renderer\src\shaders\shaderobject.h>
#include <core\logging.h>
#include <renderer\src\shaders\shadertype.h>
#include <renderer\src\gl_core_3_3.hpp>
#include <renderer\src\gltypeconversion.h>
#include <renderer\device.h>

using namespace Kandy::Renderer;

//----------------------------------------------------------------

static std::string& BuiltinConstants()
{
  static bool initialised = false;
  static std::string builtinConstants;

  if (!initialised)
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
    s << "#define VertexPosition      layout(location = " << VertexAttribLocation::Position << ") in\n"
      << "#define VertexPositionLow   layout(location = " << VertexAttribLocation::PositionLow << ") in\n"
      << "#define VertexPositionHigh  layout(location = " << VertexAttribLocation::PositionHigh << ") in\n"
      << "#define VertexNormal        layout(location = " << VertexAttribLocation::Normal << ") in\n"
      << "#define VertexColour        layout(location = " << VertexAttribLocation::Colour << ") in\n";
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
    initialised = true;
  }

  return builtinConstants;
}

//----------------------------------------------------------------

ShaderObject::ShaderObject(const char* src, GLenum type, const char* typeName)
  : shader(gl::CreateShader(type))
{
  // We define various constants for every shader but all shader source code must begin with
  // a "#version xxx" line. We need to remove any such line from the incoming source because the
  // pre-made constants have to come before it (obviously!). Note that this can be broken if the
  // #version line is not the very first, including comments...
  std::string sourceToCompile;
  if (0 == memcmp("#version", src, sizeof("#version")-1))
  {
    sourceToCompile = "//" + std::string(src);
  }
  else
  {
    sourceToCompile = src;
  }

  const char* sourceFiles[] = { BuiltinConstants().c_str(), sourceToCompile.c_str() };
  const GLint lengths[] = { BuiltinConstants().length(), sourceToCompile.length() };
  gl::ShaderSource(shader, 2, sourceFiles, lengths);

  gl::CompileShader(shader);

  GLint okay;
  gl::GetShaderiv(shader, gl::COMPILE_STATUS, &okay);
  if (!okay)
  {
    GLint logLength;
    gl::GetShaderiv(shader, gl::INFO_LOG_LENGTH, &logLength);
    std::vector<GLchar> log(logLength);
    gl::GetShaderInfoLog(shader, logLength, NULL, log.data());
    std::stringstream s;
    s << "could not compile " << typeName << " shader object:\n" << src << "\nLog:\n" << log.data();
    throw std::logic_error(s.str());
  }
}

//----------------------------------------------------------------

ShaderObject::~ShaderObject()
{
  gl::DeleteShader(shader);
}
