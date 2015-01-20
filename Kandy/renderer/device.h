#pragma once

#include <string>
#include <boost\shared_ptr.hpp>
#include <Kandy\renderer\shaders\shader.h>
#include <Kandy\renderer\window.h>
#include <Kandy\renderer\context.h>

/**
  @file device.h

  */

namespace Kandy
{
  namespace Renderer
  {
    namespace Device
    {
      /**
        @breif Affets the initialisation of the display system.

        When @see Initialise is called, these parameters determine the initial state of the @see
        Window which underpins the @see Application.

        Once @see Initialise is called, the dimensions of the backbuffer become fixed.
        */
      struct Configuration
      {
        // Default configuration.
        Configuration()
          : backbufferWidth(1280),
            backbufferHeight(720),
            windowMode(Window::WindowMode::Windowed)
        {
        }

        unsigned int backbufferWidth;         /** Size of the @see Application @see Window. */
        unsigned int backbufferHeight;        /** Size of the @see Application @see Window. */
        Window::WindowMode::Enum windowMode;  /** The initial display mode of the @see Application @see Window. */
      };

      /**
        @brief Setup the configuration of the device.
        */
      void Configure(const Configuration& config);

      /**
        @breif Called internally by @see Application during system start-up.
        @return true if successfully initialised, otherwise false.
        */
      bool Initialise();
      
      /**
        @brief Return the maximum number of texture units supported by the system.
        */
      size_t MaxTextureUnits();

      /**
        @breif Return the underlying window associated with this @see Application.
        */
      boost::shared_ptr<Window> GetWindow();

      boost::shared_ptr<Context> GetContext();

      /**
        @breif Swap the front- and back-buffer.

        This method is called automatically when the current frame has completed rendering.
        Do not call it from user code.
        */
      void SwapBuffers();

      /**
        @breif Create a new rendering context and make it current in the calling thread.
        */
      boost::shared_ptr<Context> NewContext();

      /**
        @brief Compile and create a new shader from source.
        */
      boost::shared_ptr<Shader> NewShader(
        const char* const vertexShaderSource,
        const char* const fragmentShaderSource);
    };
  }
}
