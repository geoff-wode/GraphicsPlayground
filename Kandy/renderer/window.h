#pragma once

#include <boost\shared_ptr.hpp>

/**
  @file window.h

  */

namespace Kandy
{
  namespace Renderer
  {
    struct Context;

    struct Window
    {
      virtual boost::shared_ptr<Context> GetContext() = 0;

      virtual void Show() = 0;
      virtual void Hide() = 0;
      virtual bool IsVisible() const = 0;

      struct WindowMode
      {
        enum Enum
        {
          Windowed,       /** Windowed mode. */
          RealFullScreen, /** Enter full screen mode with explicit video mode change. */
          FakeFullScreen  /** Fake full screen mode using current desktop settings. */
        };
      };

      virtual void SetMode(WindowMode::Enum mode) = 0;
      virtual bool IsFullScreen() const = 0;

    protected:
      ~Window() { }
    };
  }
}
