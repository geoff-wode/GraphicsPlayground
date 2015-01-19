#include <exception>
#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream>
#include <boost\thread.hpp>
#include <Kandy\core\logging.h>
#include <Kandy\renderer\device.h>
#include "shaderobject.h"
#include "..\gl_core_3_3.hpp"

using namespace Kandy::Core;
using namespace Kandy::Renderer;

//---------------------------------------------------

namespace
{
  boost::once_flag onceFlag = BOOST_ONCE_INIT;

  std::string builtinConstants;

  void InitialiseBuiltinConstants()
  {
    std::stringstream s;
    s <<  "#version 330 \n"
      <<  "#define VertexPosition           layout(location = 0) in\n"
      <<  "#define VertexPositionLow        layout(location = 0) in\n"
      <<  "#define VertexPositionHigh       layout(location = 1) in\n"
      <<  "#define VertexNormal             layout(location = 2) in\n"
      <<  "#define VertexColour             layout(location = 3) in\n";
    for (size_t i = Device::MaxTextureUnits(); i >= 0; --i)
    {
      s << "#define VertexTextureCoord" << i << "   layout(location = " << 4 + i << ") in\n";
    }
    s <<  "const float E             = " << (float)(M_E       ) << " // e"
      <<  "const float LOG2E         = " << (float)(M_LOG2E   ) << " // log2(e)"
      <<  "const float LOG10E        = " << (float)(M_LOG10E  ) << " // log10(e)"
      <<  "const float LN2           = " << (float)(M_LN2     ) << " // ln(2)"
      <<  "const float LN10          = " << (float)(M_LN10    ) << " // ln(10)"
      <<  "const float PI            = " << (float)(M_PI      ) << " // pi"
      <<  "const float PIover2       = " << (float)(M_PI_2    ) << " // pi/2"
      <<  "const float PIover4       = " << (float)(M_PI_4    ) << " // pi/4"
      <<  "const float OneOverPI     = " << (float)(M_1_PI    ) << " // 1/pi"
      <<  "const float TwoOverPI     = " << (float)(M_2_PI    ) << " // 2/pi"
      <<  "const float TwoOverSqrtPI = " << (float)(M_2_SQRTPI) << " // 2/sqrt(pi)"
      <<  "const float Sqrt2         = " << (float)(M_SQRT2   ) << " // sqrt(2)"
      <<  "const float OneOverSqrt2  = " << (float)(M_SQRT1_2 ) << " // 1/sqrt(2)";
    builtinConstants = s.str();
  }

  //---------------------------------------------------

  GLenum GLShaderType(ShaderObject::Type::Enum type)
  {
    switch (type)
    {
    case ShaderObject::Type::VertexShader: return gl::VERTEX_SHADER; break;
    case ShaderObject::Type::FragmentShader: return gl::FRAGMENT_SHADER; break;
    default: ASSERT(false);
    }
    return (GLenum)0;
  }
}

//---------------------------------------------------

ShaderObject::ShaderObject(Type::Enum type, const char* const source)
  : handle(gl::CreateShader(GLShaderType(type)))
{
  boost::call_once(InitialiseBuiltinConstants, onceFlag);

  // Every shader source must have a "#version" line as the first non-comment line.
  // Remove it from the user's source as we already supply one in the constant definitions...
  std::string compilerSource;
  if (0 == strcmp("#version", source))
  {
    compilerSource = "//" + std::string(source);
  }
  else
  {
    compilerSource = source;
  }

  const char* sourceParts[] = { builtinConstants.c_str(), compilerSource.c_str() };
  const GLint sourceLengths[] = { builtinConstants.length(), compilerSource.length() };
  gl::ShaderSource(handle, 2, sourceParts, sourceLengths);
  gl::CompileShader(handle);
  GLint compiledOkay;
  gl::GetShaderiv(handle, gl::COMPILE_STATUS, &compiledOkay);
  if (!compiledOkay)
  {
    GLint logLength;
    gl::GetShaderiv(handle, gl::INFO_LOG_LENGTH, &logLength);
    std::vector<GLchar> log(logLength);
    gl::GetShaderInfoLog(handle, logLength, NULL, log.data());
    WARNING("cannot compile shader object:\n%s\n", log.data());
    throw new std::logic_error("cannot compile shader object");
  }
}

//---------------------------------------------------

ShaderObject::~ShaderObject()
{
  gl::DeleteShader(handle);
}
