#include <Windows.h>
#include <SDL.h>
#include <vector>
#include <stdlib.h>
#include <glm\glm.hpp>
#include <core\logging.h>
#include <core\game.h>
#include <renderer\device.h>
#include "gl_core_3_3.hpp"
#include "gltypeconversion.h"
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

  size_t activeVertexAttribCount;
};

//------------------------------------------------------------

static void SetPipelineState(const PipelineState& state);
static void SetClearState(const ClearState& state);

static void ApplyRenderState(const RenderState& in, RenderState& out);
static void EnableState(GLenum state, bool on);
static void ApplyColourMask(const glm::bvec4& in, glm::bvec4& out);
static void ApplyDepthMask(bool in, bool& out);
static void ApplyFacetCulling(const FacetCulling& in, FacetCulling& out);
static void ApplyRasterisationMode(const RasterisationMode::Enum& in, RasterisationMode::Enum& out);
static void ApplyDepthTest(const DepthTest& in, DepthTest& out);
static void ApplyVertexArray(const VertexArray* in, size_t& activeVertexAttribCount);

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

size_t Device::MaxVertexAttributes()
{
  static GLint max = 0;
  if (!max)
  {
    gl::GetIntegerv(gl::MAX_VERTEX_ATTRIBS, &max);
  }
  return max;
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
  
  if (!impl->sdlWindow || !impl->glContext) 
  {
    MessageBox(NULL, SDL_GetError(), "ERROR", MB_OK | MB_ICONERROR | MB_TASKMODAL);
    FATAL("cannot create game window - %s\n", SDL_GetError());
    game->Exit();
    return;
  }

  if (!gl::sys::LoadFunctions())
  {
    MessageBox(NULL, "Cannot load OpenGL", "ERROR", MB_OK | MB_ICONERROR | MB_TASKMODAL);
    FATAL("cannot load OpenGL\n", 0);
    game->Exit();
    return;
  }

  INFO("OpenGL %d.%d\n", gl::sys::GetMajorVersion(), gl::sys::GetMinorVersion());
  BOOST_FOREACH(auto attr, glAttrs)
  {
    SDL_GL_GetAttribute(attr.id, &attr.actualValue);
    INFO("%s = %d (%d requested)\n", attr.name, attr.actualValue, attr.preferredValue);
  }

  // Force the GL state into our defaults...
  SetClearState(impl->clearState);
  SetPipelineState(impl->renderState.pipelineState);
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

  gl::Clear(ToGL(buffers));
}

//------------------------------------------------------------

void Device::Render(PrimitiveType::Enum primitive, int count, const RenderState& renderState)
{
  Render(primitive, count, 0, renderState);
}

//------------------------------------------------------------

void Device::Render(PrimitiveType::Enum primitive, int count, int offset, const RenderState& renderState)
{
  ApplyRenderState(renderState, impl->renderState);
  if (renderState.vertexArray != impl->renderState.vertexArray)
  {
    ApplyVertexArray(renderState.vertexArray, impl->activeVertexAttribCount);
    impl->renderState.vertexArray = renderState.vertexArray;
  }
  // TODO: ApplyShader
  // TODO: ApplyTextureUnits
  // TODO: ApplyFrameBuffers
}

//------------------------------------------------------------

static void ApplyRenderState(const RenderState& in, RenderState& out)
{
  ApplyColourMask(in.pipelineState.colourMask, out.pipelineState.colourMask);
  ApplyDepthMask(in.pipelineState.depthMask, out.pipelineState.depthMask);
  ApplyFacetCulling(in.pipelineState.faceCulling, out.pipelineState.faceCulling);
  ApplyRasterisationMode(in.pipelineState.rasterisationMode, out.pipelineState.rasterisationMode);
  ApplyDepthTest(in.pipelineState.depthTest, out.pipelineState.depthTest);
}

//------------------------------------------------------------

static void EnableState(GLenum state, bool on)
{
  on ? gl::Enable(state) : gl::Disable(state);
}

//------------------------------------------------------------

static void SetClearState(const ClearState& state)
{
  const glm::vec4& colour = state.colourBuffer.colour;
  gl::ClearColor(colour.r, colour.g, colour.b, colour.a);
  gl::ClearDepth(state.depthBuffer.value);
}

//------------------------------------------------------------

static void SetPipelineState(const PipelineState& state)
{
  EnableState(gl::CULL_FACE, state.faceCulling.enabled);
  gl::CullFace(ToGL(state.faceCulling.cullFace));
  gl::FrontFace(ToGL(state.faceCulling.frontFaceWindingOrder));

  EnableState(gl::DEPTH_TEST, state.depthTest.enabled);
  gl::DepthFunc(ToGL(state.depthTest.function));

  gl::ColorMask(state.colourMask.r, state.colourMask.g, state.colourMask.b, state.colourMask.a);
  gl::DepthMask(state.depthMask ? gl::TRUE_ : gl::FALSE_);
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

//------------------------------------------------------------

static void ApplyFacetCulling(const FacetCulling& in, FacetCulling& out)
{
  if (in.enabled != out.enabled)
  {
    EnableState(gl::CULL_FACE, in.enabled);
    out.enabled = in.enabled;
  }
  if (in.enabled)
  {
    if (in.cullFace != out.cullFace)
    {
      gl::CullFace(ToGL(in.cullFace));
      out.cullFace = in.cullFace;
    }
    if (in.frontFaceWindingOrder != out.frontFaceWindingOrder)
    {
      gl::FrontFace(ToGL(in.frontFaceWindingOrder));
      out.frontFaceWindingOrder = in.frontFaceWindingOrder;
    }
  }
}

//------------------------------------------------------------

static void ApplyRasterisationMode(const RasterisationMode::Enum& in, RasterisationMode::Enum& out)
{
  if (in != out)
  {
    gl::PolygonMode(gl::FRONT_AND_BACK, ToGL(in));
    out = in;
  }
}

//------------------------------------------------------------

static void ApplyDepthTest(const DepthTest& in, DepthTest& out)
{
  if (in.enabled != out.enabled)
  {
    EnableState(gl::DEPTH_TEST, in.enabled);
    out.enabled = in.enabled;
  }
  if (in.enabled)
  {
    if (in.function != out.function)
    {
      gl::DepthFunc(ToGL(in.function));
      out.function = in.function;
    }
  }
}

//------------------------------------------------------------

static void ApplyVertexArray(const VertexArray* in, size_t& activeVertexAttribCount)
{
  // Enable all the attributes for the new vertex array...
  in->Bind();
  const std::vector<VertexArrayAttribute>& inAttrs = in->GetAttributes();
  for (size_t index = 0; index < inAttrs.size(); ++index)
  {
    gl::EnableVertexAttribArray(index);
    gl::VertexAttribPointer(
      index,
      inAttrs[index].SizeInBytes(),
      ToGL(inAttrs[index].type),
      inAttrs[index].normalise,
      in->Stride(),
      (const GLvoid*)inAttrs[index].offset);
  }

  // If the old vertex array had more attributes than the new one does, disable them...
  for (size_t index = inAttrs.size(); index < activeVertexAttribCount; ++index)
  {
    gl::DisableVertexAttribArray(index);
  }
  activeVertexAttribCount = inAttrs.size();
}

//------------------------------------------------------------

