#include <string>
#include <core\logging.h>
#include <renderer\window.h>

using namespace Kandy::Renderer;

Window::Window()
{
}

Window::~Window()
{
  if (context) { SDL_GL_DeleteContext(context); }
  if (window) { SDL_DestroyWindow(window); }
}

boost::shared_ptr<Window> Window::Create(const glm::ivec2& size, const char* title)
{
  boost::shared_ptr<Window> win(new Window());

  if (win)
  {
    const std::string signature(std::string(title) + " (Kandy)");
    win->window = SDL_CreateWindow(
      signature.c_str(),
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      size.x, size.y,
      SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    if (win->window)
    {
      win->context = SDL_GL_CreateContext(win->window);
      if (win->context)
      {
        SDL_GL_SetSwapInterval(1);
      }
    }

    LOG("%s created for game '%s'\n", !win->window ? "no window" : !win->context ? "no context" : "window", title);
    if (!win->window || !win->context)
    {
      win.reset();
    }
  }

  return win;
}

void Window::SetCurrent()
{
  SDL_assert(window && context);
  SDL_GL_MakeCurrent(window, context);
}

void Window::Show()
{
  SDL_assert(window && context);
  SDL_ShowWindow(window);
}

void Window::Hide()
{
  SDL_assert(window && context);
  SDL_HideWindow(window);
}

void Window::SwapBuffers()
{
  SDL_assert(window && context);
  SDL_GL_SwapWindow(window);
}
