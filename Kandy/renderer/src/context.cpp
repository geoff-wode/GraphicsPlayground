#include <exception>
#include <SDL.h>
#include <boost\thread.hpp>
#include <Kandy\core\logging.h>
#include <Kandy\renderer\context.h>
#include "contextimpl.h"
#include "windowimpl.h"
#include "gl_core_3_3.hpp"

using namespace Kandy::Core;
using namespace Kandy::Renderer;

//----------------------------------------------------

#define GL_ATTR(id)   { #id, id }

//---------------------------------------------------------

namespace
{
  boost::once_flag onceFlag = BOOST_ONCE_INIT;

  const struct GLAttr
  {
    const char* const name;
    const SDL_GLattr  id;
  } glAttrs[] =
  {
    GL_ATTR(SDL_GL_CONTEXT_MAJOR_VERSION),
    GL_ATTR(SDL_GL_CONTEXT_MINOR_VERSION),
    GL_ATTR(SDL_GL_DOUBLEBUFFER),
    GL_ATTR(SDL_GL_RED_SIZE),
    GL_ATTR(SDL_GL_GREEN_SIZE),
    GL_ATTR(SDL_GL_BLUE_SIZE),
    GL_ATTR(SDL_GL_ALPHA_SIZE),
    GL_ATTR(SDL_GL_DEPTH_SIZE),
    GL_ATTR(SDL_GL_STENCIL_SIZE),
    GL_ATTR(SDL_GL_ACCUM_RED_SIZE),
    GL_ATTR(SDL_GL_ACCUM_GREEN_SIZE),
    GL_ATTR(SDL_GL_ACCUM_BLUE_SIZE),
    GL_ATTR(SDL_GL_ACCUM_ALPHA_SIZE),
    GL_ATTR(SDL_GL_MULTISAMPLEBUFFERS),
    GL_ATTR(SDL_GL_MULTISAMPLESAMPLES),
    GL_ATTR(SDL_GL_SHARE_WITH_CURRENT_CONTEXT)
  };
  const size_t glAttrCount = sizeof(glAttrs) / sizeof(glAttrs[0]);
}

//-------------------------------------------------------------------

static void InitialiseGL();
static void ConfigureInitialRenderState(const RenderState& state);

//----------------------------------------------------

ContextImpl::ContextImpl(struct WindowImpl* const parentWindow)
  : parentWindow(parentWindow),
    glContext(SDL_GL_CreateContext(parentWindow->sdlWindow))
{
  if (!glContext) { throw new std::logic_error(SDL_GetError()); }
  INFO("created context %p\n", this);

  ConfigureInitialRenderState(renderState);
}

//----------------------------------------------------

ContextImpl::~ContextImpl()
{
  SDL_GL_DeleteContext(glContext);
  INFO("destroyed context %p\n", this);
}

//----------------------------------------------------
void ContextImpl::MakeCurrent()
{
  ASSERT(glContext);
  SDL_GL_MakeCurrent(parentWindow->sdlWindow, glContext);
}

//----------------------------------------------------

Window& ContextImpl::GetParentWindow() const
{
  return *parentWindow;
}

//----------------------------------------------------

void ContextImpl::Clear(const ClearState& clearState)
{
}

//----------------------------------------------------

void ContextImpl::Draw(const RenderState& drawState, const SceneState& sceneState)
{
}

//----------------------------------------------------

static void ConfigureInitialRenderState(const RenderState& state)
{
  boost::call_once(InitialiseGL, onceFlag);

  gl::DepthMask(state.depthMask);
  gl::ColorMask(state.colourMask.r, state.colourMask.g, state.colourMask.b, state.colourMask.a);
}

//----------------------------------------------------

static void InitialiseGL()
{
  if (!gl::sys::LoadFunctions())
  {
    throw new std::logic_error("cannot find GL functions");
  }

  INFO("OpenGL %d.%d\n", gl::sys::GetMajorVersion(), gl::sys::GetMinorVersion());
  INFO("GLSL: %s\n", gl::GetString(gl::SHADING_LANGUAGE_VERSION));
  for (size_t i = 0; i < glAttrCount; ++i)
  {
    int value;
    SDL_GL_GetAttribute(glAttrs[i].id, &value);
    INFO("%-12s: %d\n", glAttrs[i].name, value);
  }
}
