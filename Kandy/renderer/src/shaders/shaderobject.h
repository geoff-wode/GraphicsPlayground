#pragma once

#include <renderer\src\shaders\shadertype.h>
#include <renderer\src\gl_core_3_3.hpp>
#include <renderer\src\gltypeconversion.h>

namespace Kandy
{
  namespace Renderer
  {
    class ShaderObject
    {
    public:
      virtual ~ShaderObject();

      GLuint shader;

    protected:
      ShaderObject(const char* src, GLenum type, const char* typeName);
    };

    //--------------------------------------------------------------

    class VertexShaderObject : public ShaderObject
    {
    public:
      VertexShaderObject(const char* src)
        : ShaderObject(src, ToGL(ShaderType::Vertex), "Vertex")
      {
      }
    };

    //--------------------------------------------------------------

    class GeometryShaderObject : public ShaderObject
    {
    public:
      GeometryShaderObject(const char* src)
        : ShaderObject(src, ToGL(ShaderType::Geometry), "Geometry")
      {
      }
    };

    //--------------------------------------------------------------

    class FragmentShaderObject : public ShaderObject
    {
    public:
      FragmentShaderObject(const char* src)
        : ShaderObject(src, ToGL(ShaderType::Fragment), "Fragment")
      {
      }
    };
  }
}
