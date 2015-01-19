#pragma once

/**
  @file shaderuniformobserver.h

  */

namespace Kandy
{
  namespace Renderer
  {
    struct ShaderUniform;

    /**
      An interface for observers of events happening to @see ShaderUniform values.
      */
    struct ShaderUniformObserver
    {
      /**
        Called by an @see ShaderUniform when its value is modified.
        */
      virtual void Modified(const ShaderUniform* const uniform) = 0;
    };
  }
}
