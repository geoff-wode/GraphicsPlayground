#pragma once

#include <string>

/**
  @file shaderobject.h

  */

namespace Kandy
{
  namespace Renderer
  {
    struct ShaderObject
    {
      struct Type
      {
        enum Enum
        {
          VertexShader,
          FragmentShader
        };
      };

      ShaderObject(Type::Enum type, const char* const source);

      ~ShaderObject();

      std::string GetCompilerLog() const;

      const unsigned int handle;
    };
  }
}
