#pragma once

#include <SDL.h>
#include <boost\shared_ptr.hpp>
#include <Kandy\renderer\window.h>
#include "contextimpl.h"

/**
  @file window.h

  */

namespace Kandy
{
  namespace Renderer
  {
    struct WindowImpl : public Window
    {
      SDL_Window* sdlWindow;

      WindowImpl(unsigned int width, unsigned int height, WindowMode::Enum mode);
      virtual ~WindowImpl();

      /**
        @breif Swap the front- and back-buffer.
        */
      void Swap();

      // Implementation of the external API...
      virtual void Show();
      virtual void Hide();
      virtual bool IsVisible() const;
      virtual void SetMode(WindowMode::Enum mode);
      virtual bool IsFullScreen() const;

    };
  }
}
