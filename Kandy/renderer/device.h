#pragma once

#include <string>
#include <boost\shared_ptr.hpp>
#include <Kandy\renderer\shaders\shader.h>
#include <Kandy\renderer\window.h>

/**
  @file device.h

  */

namespace Kandy
{
  namespace Renderer
  {
    namespace Device
    {
      void Initialise();
      
      size_t MaxTextureUnits();

      boost::shared_ptr<Window> NewWindow(unsigned int width, unsigned int height);

      boost::shared_ptr<Shader> NewShader(
        const char* const vertexShaderSource,
        const char* const fragmentShaderSource);
    };
  }
}
