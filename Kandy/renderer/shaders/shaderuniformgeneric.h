#pragma once

#include <Kandy\renderer\shaders\shaderuniform.h>

/**
  @file shaderuniformgeneric.h

  */

namespace Kandy
{
  namespace Renderer
  {
    /**
      A generic class used to create type-safe specialisations of @see ShaderUniform.
      */
    template <typename T>
    struct ShaderUniformGeneric : public ShaderUniform
    {
      /**
        Set the uniform's value.

        No data is transferred to the GPU in this method: the data are cached in system
        RAM until the uniform's parent shader is actually called upon to do some rendering.
        At that point, any uniforms which have been modified since the last render call have
        their data uploaded to the GPU.
        */
      void Set(const T& newValue);

      /**
        Retreive the current (cached) value.

        There is never any GPU interaction resulting from this method.
        */
      T Get() const { return value; }

    protected:
      T value;

      ShaderUniformGeneric(unsigned int location, const char* const name, ShaderUniformObserver* const observer)
        : ShaderUniform(location, name, observer)
      {
      }
    };
  }
}
