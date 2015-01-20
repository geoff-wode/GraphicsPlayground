#include <exception>
#include <SDL.h>
#include <boost\make_shared.hpp>
#include <Kandy\core\logging.h>
#include <Kandy\renderer\window.h>
#include "contextimpl.h"
#include "gl_core_3_3.hpp"
#include "windowimpl.h"

using namespace Kandy::Core;
using namespace Kandy::Renderer;


//---------------------------------------------------------

WindowImpl::WindowImpl(unsigned int width, unsigned int height, WindowMode::Enum mode)
{
  Uint32 flags = SDL_WINDOW_OPENGL;
  switch (mode)
  {
  case WindowMode::FakeFullScreen: flags |= SDL_WINDOW_FULLSCREEN_DESKTOP; break;
  case WindowMode::RealFullScreen: flags |= SDL_WINDOW_FULLSCREEN; break;
  case WindowMode::Windowed:       break;
  default: ASSERT(false);
  }

  sdlWindow = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
  if (!sdlWindow) { throw new std::logic_error(SDL_GetError()); }
  INFO("created window %p\n", this);
}

//---------------------------------------------------------

WindowImpl::~WindowImpl()
{
  SDL_DestroyWindow(sdlWindow);
  INFO("destroyed window %p\n", this);
}

//---------------------------------------------------------

void WindowImpl::Swap()
{
  SDL_GL_SwapWindow(sdlWindow);
}

//---------------------------------------------------------

void WindowImpl::Show()
{
  SDL_ShowWindow(sdlWindow);
}

//---------------------------------------------------------

void WindowImpl::Hide()
{
  SDL_HideWindow(sdlWindow);
}

//---------------------------------------------------------

bool WindowImpl::IsVisible() const
{
  return (SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_SHOWN) != 0;
}

//---------------------------------------------------------

void WindowImpl::SetMode(WindowMode::Enum mode)
{
  switch (mode)
  {
  case WindowMode::Windowed:       SDL_SetWindowFullscreen(sdlWindow, 0); break;
  case WindowMode::RealFullScreen: SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN); break;
  case WindowMode::FakeFullScreen: SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP); break;
  default:
    WARNING("invalid window mode asked for (%d)\n", mode);
    ASSERT(false);
  }
}

//---------------------------------------------------------

bool WindowImpl::IsFullScreen() const
{
  return (SDL_GetWindowFlags(sdlWindow) & SDL_WINDOW_FULLSCREEN) != 0;
}

//---------------------------------------------------------
