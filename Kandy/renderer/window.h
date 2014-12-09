#pragma once

#include <SDL.h>
#include <glm\glm.hpp>
#include <boost\shared_ptr.hpp>
#include <boost\noncopyable.hpp>

namespace Kandy
{
  namespace Renderer
  {
    class Window : public boost::noncopyable
    {
    public:
      static boost::shared_ptr<Window> Create(const glm::ivec2& size, const char* title);

      ~Window();

      void SetCurrent();
      void Show();
      void Hide();
      void SwapBuffers();

    private:
      Window();

      SDL_Window* window;
      SDL_GLContext context;
    };
  }
}
