#pragma once

#include <string>

/**
  @file shaderuniform.h

  */

namespace Kandy
{
  namespace Renderer
  {
    struct ShaderUniformObserver;

    struct ShaderUniform
    {
      virtual void Apply() = 0;

      const unsigned int Location;
      const std::string Name;

    protected:
      ShaderUniformObserver* const observer;

      ShaderUniform(unsigned int location, const char* const name, ShaderUniformObserver* const observer);
    };
  }
}
