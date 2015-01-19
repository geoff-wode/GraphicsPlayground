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

WindowImpl::WindowImpl(unsigned int width, unsigned int height)
  : sdlWindow(SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
      SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN))
{
  if (!sdlWindow) { throw new std::logic_error(SDL_GetError()); }
  context = boost::make_shared<ContextImpl>(this);
  INFO("created window %p\n", this);
  context->MakeCurrent();
}

//---------------------------------------------------------

WindowImpl::~WindowImpl()
{
  SDL_DestroyWindow(sdlWindow);
  INFO("destroyed window %p\n", this);
}

//---------------------------------------------------------

boost::shared_ptr<Context> WindowImpl::GetContext()
{
  return context;
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
