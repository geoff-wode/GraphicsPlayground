#pragma once

#include <string>
#include <set>
#include <boost\shared_ptr.hpp>
#include <Kandy\renderer\shaders\shader.h>
#include <Kandy\renderer\shaders\shaderuniformobserver.h>

/**
  @file shaderimpl.h

  */

namespace Kandy
{
  namespace Renderer
  {
    struct ShaderUniform;

    struct ShaderImpl : public Shader, public ShaderUniformObserver
    {
      /**
        Create a new shader instance.
        */
        ShaderImpl(
        const char* const vertexShaderSource,
        const char* const fragmentShaderSource);

      virtual ~ShaderImpl();
      
      virtual Uniforms& GetUniforms() { return uniforms; }

      virtual void Bind();

      /**
        Copies all uniform values which have been modified since the last call to the GPU.
        */
      void Apply();

      /**
        Called by an individual @see ShaderUniform when it's value is changed.
        */
      virtual void Modified(const ShaderUniform* const uniform);

      const unsigned int handle;   /** Shader program handle. */

      /**
        The set of uniforms modified since the last time this shader was updated.
        */
      std::set<const ShaderUniform* const> changedUniforms;

      /**
        The set of uniforms discovered in this shader.
        */
      Uniforms uniforms;
    };
  }
}
