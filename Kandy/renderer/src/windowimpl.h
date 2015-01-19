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
      boost::shared_ptr<ContextImpl> context;

      WindowImpl(unsigned int width, unsigned int height);
      virtual ~WindowImpl();
      virtual boost::shared_ptr<Context> GetContext();
      virtual void Show();
      virtual void Hide();
      virtual bool IsVisible() const;
      virtual void SetMode(WindowMode::Enum mode);
      virtual bool IsFullScreen() const;
    };
  }
}
