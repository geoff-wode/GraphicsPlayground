#include <Windows.h>
#include <SDL.h>
#include <vector>
#include <stdlib.h>
#include <glm\glm.hpp>
#include <core\logging.h>
#include <core\game.h>
#include <renderer\device.h>
#include "gl_core_3_3.hpp"
#include <boost\foreach.hpp>
#include <boost\shared_ptr.hpp>
#include <boost\make_shared.hpp>
#include <boost\foreach.hpp>

using namespace Kandy::Core;
using namespace Kandy::Renderer;

//------------------------------------------------------------

#define GL_ATTR(container, id, preferredValue) \
  do \
  { \
    GLAttr attr(#id, id, preferredValue); \
    container.push_back(attr); \
  } while (0)

//------------------------------------------------------------

static DeviceContext defaultContext;

//------------------------------------------------------------

struct GLAttr
{
  GLAttr(const char* const name, const SDL_GLattr id, int preferredValue)
    : name(name), id(id), preferredValue(preferredValue)
  {
  }
  const char* const name;
  const SDL_GLattr  id;
  const int preferredValue;
  int actualValue;
};

//------------------------------------------------------------

struct Device::Impl
{
  Impl()
    : fullscreen(false),
      backbufferSize(1280, 720),
      deviceContext(defaultContext)
  {
  }

  bool fullscreen;
  glm::ivec2 backbufferSize;
  DeviceContext deviceContext;
  SDL_Window* sdlWindow;
  SDL_GLContext glContext;
  ClearState clearState;
  RenderState renderState;
};

//------------------------------------------------------------

static void ApplyColourMask(const glm::bvec4& in, glm::bvec4& out);
static void ApplyDepthMask(bool in, bool& out);

//------------------------------------------------------------

Device::Device() : impl(new Impl())
{
}

//------------------------------------------------------------

Device::~Device()
{
  SDL_GL_DeleteContext(impl->glContext);
  SDL_DestroyWindow(impl->sdlWindow);
  delete impl;
}

//------------------------------------------------------------

void Device::SetContext(const DeviceContext& context) { impl->deviceContext = context; }

//------------------------------------------------------------

void Device::SetFullScreen(bool enable)
{
  if (impl->fullscreen != enable)
  {
    if (enable)
    {
      const Uint32 flags = impl->deviceContext.fakeFullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_FULLSCREEN;
      SDL_SetWindowFullscreen(impl->sdlWindow, flags);
    }
    else
    {
      SDL_SetWindowFullscreen(impl->sdlWindow, 0);
    }
    impl->fullscreen = enable;
  }
}

//------------------------------------------------------------

void Device::Initialise(Game* const game)
{
  // See these sites for details on setting GL attributes _for the main render window_:
  // http://wiki.libsdl.org/SDL_GLattr#OpenGL
  // http://stackoverflow.com/questions/22435518/sdl2-and-glew-unable-to-get-proper-opengl-version-if-using-sdl-gl-setattribute
  std::vector<GLAttr> glAttrs;
  //GL_ATTR(glAttrs, SDL_GL_CONTEXT_PROFILE_MASK,  SDL_GL_CONTEXT_PROFILE_CORE);
  //GL_ATTR(glAttrs, SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
  //GL_ATTR(glAttrs, SDL_GL_CONTEXT_FLAGS,         SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  //GL_ATTR(glAttrs, SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  //GL_ATTR(glAttrs, SDL_GL_CONTEXT_MINOR_VERSION, 3);
  //GL_ATTR(glAttrs, SDL_GL_RED_SIZE,              impl->deviceContext.colourDepth.r);
  //GL_ATTR(glAttrs, SDL_GL_GREEN_SIZE,            impl->deviceContext.colourDepth.g);
  //GL_ATTR(glAttrs, SDL_GL_BLUE_SIZE,             impl->deviceContext.colourDepth.b);
  //GL_ATTR(glAttrs, SDL_GL_ALPHA_SIZE,            impl->deviceContext.colourDepth.a);
  GL_ATTR(glAttrs, SDL_GL_DOUBLEBUFFER,          impl->deviceContext.doubleBuffered ? 1 : 0);

  BOOST_FOREACH(auto attr, glAttrs)
  {
    SDL_GL_SetAttribute(attr.id, attr.preferredValue);
  }

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
  {
    MessageBox(NULL, SDL_GetError(), "ERROR", MB_OK | MB_ICONERROR | MB_TASKMODAL);
    FATAL("cannot initialise system - %s\n", SDL_GetError());
    game->Exit();
    return;
  }
  atexit(SDL_Quit);

  char path[MAX_PATH + 1];
  const DWORD pathSize = GetModuleFileName(NULL, path, MAX_PATH);

  impl->sdlWindow = SDL_CreateWindow(pathSize ? path : "KandyApp",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    impl->backbufferSize.x, impl->backbufferSize.y,
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | (impl->fullscreen ? SDL_WINDOW_FULLSCREEN : 0));

  if (impl->sdlWindow)
  {
    impl->glContext = SDL_GL_CreateContext(impl->sdlWindow);
  }
  
  if (impl->sdlWindow && impl->glContext)
  {
    if (gl::sys::LoadFunctions())
    {
      INFO("OpenGL %d.%d\n", gl::sys::GetMajorVersion(), gl::sys::GetMinorVersion());
      BOOST_FOREACH(auto attr, glAttrs)
      {
        SDL_GL_GetAttribute(attr.id, &attr.actualValue);
        INFO("%s = %d (%d requested)\n", attr.name, attr.actualValue, attr.preferredValue);
      }
    }
    else
    {
      MessageBox(NULL, "Cannot load OpenGL", "ERROR", MB_OK | MB_ICONERROR | MB_TASKMODAL);
      FATAL("cannot load OpenGL\n", 0);
      game->Exit();
    }
  }
  else
  {
    MessageBox(NULL, SDL_GetError(), "ERROR", MB_OK | MB_ICONERROR | MB_TASKMODAL);
    FATAL("cannot create game window - %s\n", SDL_GetError());
    game->Exit();
  }
}

//------------------------------------------------------------

void Device::PresentBackbuffer()
{
  SDL_GL_SwapWindow(impl->sdlWindow);
}

//------------------------------------------------------------

void Device::Clear(const ClearState::Buffers::Enum& buffers, const ClearState& state)
{
  ApplyColourMask(state.colourBuffer.mask, impl->clearState.colourBuffer.mask);
  ApplyDepthMask(state.depthBuffer.mask, impl->clearState.depthBuffer.mask);

  if (state.colourBuffer.colour != impl->clearState.colourBuffer.colour)
  {
    const glm::vec4& colour = state.colourBuffer.colour;
    gl::ClearColor(colour.r, colour.g, colour.b, colour.a);
    impl->clearState.colourBuffer.colour = colour;
  }

  if (state.depthBuffer.value != impl->clearState.depthBuffer.value)
  {
    gl::ClearDepth(state.depthBuffer.value);
    impl->clearState.depthBuffer.value = state.depthBuffer.value;
  }

  GLbitfield bufferBits = 0;
  if (buffers & ClearState::Buffers::Colour) { bufferBits |= gl::COLOR_BUFFER_BIT; }
  if (buffers & ClearState::Buffers::Depth) { bufferBits |= gl::DEPTH_BUFFER_BIT; }
  if (buffers & ClearState::Buffers::Stencil) { bufferBits |= gl::STENCIL_BUFFER_BIT; }
  gl::Clear(bufferBits);
}

//------------------------------------------------------------

static void ApplyColourMask(const glm::bvec4& in, glm::bvec4& out)
{
  if (in != out)
  {
    gl::ColorMask(in.r, in.g, in.b, in.a);
    out = in;
  }
}

//------------------------------------------------------------

static void ApplyDepthMask(bool in, bool& out)
{
  if (in != out)
  {
    gl::DepthMask(in ? gl::TRUE_ : gl::FALSE_);
    out = in;
  }
}
