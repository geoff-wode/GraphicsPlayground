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

namespace
{
  void ConfigureInitialRenderState(const RenderState& state)
  {
    gl::DepthMask(state.depthMask);
    gl::ColorMask(state.colourMask.r, state.colourMask.g, state.colourMask.b, state.colourMask.a);
  }

  //----------------------------------------------------

  void InitialiseGL()
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

  //----------------------------------------------------

  GLenum ClearBuffersToGL(const ClearState::Buffers::Enum buffers)
  {
    GLenum mask = 0;
    if (buffers & ClearState::Buffers::Colour)  { mask |= gl::COLOR_BUFFER_BIT; }
    if (buffers & ClearState::Buffers::Depth)   { mask |= gl::DEPTH_BUFFER_BIT; }
    if (buffers & ClearState::Buffers::Stencil) { mask |= gl::STENCIL_BUFFER_BIT; }
    return mask;
  }

  //----------------------------------------------------

  void SetColourMask(const glm::bvec4& newValue, glm::bvec4& value)
  {
    if (newValue != value)
    {
      value = newValue;
      gl::ColorMask(value.r, value.g, value.b, value.a);
    }
  }

  //----------------------------------------------------

  void SetDepthMask(const bool newValue, bool& value)
  {
    if (newValue != value)
    {
      value = newValue;
      gl::DepthMask(value);
    }
  }
}

//----------------------------------------------------

ContextImpl::ContextImpl(WindowImpl* const parentWindow)
  : parentWindow(parentWindow),
    glContext(SDL_GL_CreateContext(parentWindow->sdlWindow))
{
  if (!glContext) { throw new std::logic_error(SDL_GetError()); }
  INFO("created context %p\n", this);

  // GL functions must be bound in _after_ a context is created but, obviously(!),
  // before any actual GL calls are made. They also are a "global" binding so this
  // only needs to happen once in the life time of the application.
  boost::call_once(InitialiseGL, onceFlag);

  // Set the context to the default state.
  ConfigureInitialRenderState(currentRenderState);
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
  SetColourMask(clearState.colourMask, currentClearState.colourMask);
  SetDepthMask(clearState.depthMask, currentClearState.depthMask);

  if (clearState.colour != currentClearState.colour)
  {
    currentClearState.colour = clearState.colour;
    gl::ClearColor(currentClearState.colour.r, currentClearState.colour.g, currentClearState.colour.b, currentClearState.colour.a);
  }

  if (clearState.depth != currentClearState.depth)
  {
    currentClearState.depth = clearState.depth;
    gl::ClearDepth(currentClearState.depth);
  }
  
  gl::Clear(ClearBuffersToGL(clearState.buffers));
}

//----------------------------------------------------

void ContextImpl::Draw(const RenderState& renderState, const SceneState& sceneState)
{
}

//----------------------------------------------------
void ContextImpl::SetViewport(const Kandy::Core::Rectangle& rectangle)
{
  if (viewport != rectangle)
  {
    viewport = rectangle;
    gl::Viewport(viewport.x, viewport.y, viewport.width, viewport.height);
  }
}

//----------------------------------------------------

Kandy::Core::Rectangle ContextImpl::GetViewport() const
{
  return viewport;
}

//----------------------------------------------------
