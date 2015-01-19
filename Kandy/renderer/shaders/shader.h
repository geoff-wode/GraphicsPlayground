#pragma once

#include <map>
#include <boost\shared_ptr.hpp>
#include <Kandy\renderer\shaders\shaderuniform.h>

/**
  @file shader.h

  */

namespace Kandy
{
  namespace Renderer
  {
    struct Shader
    {
      typedef std::map<std::string, boost::shared_ptr<ShaderUniform> > Uniforms;
      virtual Uniforms& GetUniforms() = 0;

      /**
        Binds (makes active) this shader.
        */
      virtual void Bind() = 0;

    protected:
      ~Shader() { }
    };
  }
}
