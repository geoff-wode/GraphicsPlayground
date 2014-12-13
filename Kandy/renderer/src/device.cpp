#include <Windows.h>
#include <SDL.h>
#include <stdlib.h>
#include <boost\make_shared.hpp>
#include <boost\foreach.hpp>
#include <core\logging.h>
#include <core\game.h>
#include <renderer\device.h>
#include <renderer\shader\autouniforms\list.h>
#include "gl_core_3_3.hpp"

using namespace Kandy;
using namespace Kandy::Renderer;

//------------------------------------------------------------

AutoUniform::FactoryCollection Device::autoUniformFactories;

//------------------------------------------------------------

static void ForceClearState(const ClearState& state)
{
  gl::ClearColor(state.colour.r, state.colour.g, state.colour.b, state.colour.a);
}

//------------------------------------------------------------

static void ForceRenderState(const PipelineState& state)
{
  gl::ColorMask(state.colourMask.r, state.colourMask.g, state.colourMask.b, state.colourMask.a);
}

//------------------------------------------------------------

static void ApplyColourMask(const glm::bvec4& value, glm::bvec4& state)
{
  if (state != value)
  {
    state = value;
    gl::ColorMask(state.r, state.g, state.b, state.a);
  }
}

//------------------------------------------------------------

static void ApplyIndexBuffer(const IndexBuffer::Ptr newBuffer, IndexBuffer::Ptr& oldBuffer)
{
  if (!newBuffer && oldBuffer)
  {
    oldBuffer->Unbind();
  }
  else if (newBuffer != oldBuffer)
  {
    newBuffer->Bind();
  }
  oldBuffer = newBuffer;
}

//------------------------------------------------------------

static GLenum GLElementType(VertexElement::DataType::Enum type)
{
  switch (type)
  {
  case VertexElement::DataType::Byte:       return gl::BYTE; break;
  case VertexElement::DataType::Short:      return gl::SHORT; break;
  case VertexElement::DataType::Int:        return gl::INT; break;
  case VertexElement::DataType::Float:      return gl::FLOAT; break;
  case VertexElement::DataType::HalfFloat:  return gl::HALF_FLOAT; break;
  case VertexElement::DataType::Double:     return gl::DOUBLE; break;
  default: ASSERT(false);
  }
  return (GLenum)0;
}

//------------------------------------------------------------

static void ApplyVertexBuffer(const VertexBuffer::Ptr newBuffer, VertexBuffer::Ptr& oldBuffer)
{
  if (newBuffer != oldBuffer)
  {
    const IVertexType& newVertexType = newBuffer->VertexType;
    const VertexDeclaration& newVertexDecl = newVertexType.GetVertexDeclaration();

    // Switch to the new buffer, enabling required elements...
    newBuffer->Bind();
    for (size_t i = 0; i < newVertexDecl.ElementCount; ++i)
    {
      gl::EnableVertexAttribArray(i);
      gl::VertexAttribPointer(i,
        newVertexDecl.Elements[i].Size,
        GLElementType(newVertexDecl.Elements[i].Type),
        newVertexDecl.Elements[i].Normalise,
        newVertexDecl.Elements[i].ComponentCount,
        (const GLvoid*)newVertexDecl.Elements[i].Offset);
    }

    // Disable vertex elements which are not required by the new buffer...
    const IVertexType& oldVertexType = oldBuffer->VertexType;
    const VertexDeclaration& oldVertexDecl = oldVertexType.GetVertexDeclaration();
    if (newVertexDecl.ElementCount < oldVertexDecl.ElementCount)
    {
      for (size_t i = newVertexDecl.ElementCount; i < oldVertexDecl.ElementCount; ++i)
      {
        gl::DisableVertexAttribArray(i);
      }
    }

    oldBuffer = newBuffer;
  }
}

//------------------------------------------------------------

static void ApplyShader(const Shader::Ptr newShader, Shader::Ptr& oldShader)
{
  if (newShader != oldShader)
  {
    newShader->Use();
    oldShader = newShader;
  }
  oldShader->UpdateUniforms();
}

//------------------------------------------------------------

static void ApplyRenderState(const RenderState& newState, RenderState& oldState)
{
  ApplyColourMask(newState.pipelineState.colourMask, oldState.pipelineState.colourMask);
  ApplyIndexBuffer(newState.indexBuffer, oldState.indexBuffer);
  ApplyVertexBuffer(newState.vertexBuffer, oldState.vertexBuffer);
  ApplyShader(newState.shader, oldState.shader);
}

//------------------------------------------------------------

Device::Device()
  : colourDepth(8,8,8,8),
    backbufferSize(1280,720),
    fullScreen(false),
    multiSampling(true),
    multiSamplingBuffers(2),
    depthBufferSize(24),
    stencilBufferSize(8)
{
}

//------------------------------------------------------------

Device::~Device()
{
}

//------------------------------------------------------------

bool Device::Initialise(Core::Game& game)
{
  if (0 != SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
  {
    MessageBox(NULL, SDL_GetError(), "SDL Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
    return false;
  }
  atexit(SDL_Quit);

#define GL_ATTR(id, value)  { #id, id, value, 0 }
  struct GLAttr
  {
    const char* const name;
    const SDL_GLattr  id;
    const int         preferredValue;
    int               actualValue;
  } attrs[] =
  {
    GL_ATTR(SDL_GL_CONTEXT_MAJOR_VERSION, 3),
    GL_ATTR(SDL_GL_CONTEXT_MINOR_VERSION, 3),
    GL_ATTR(SDL_GL_CONTEXT_PROFILE_MASK,  SDL_GL_CONTEXT_PROFILE_CORE),
    GL_ATTR(SDL_GL_RED_SIZE,              colourDepth.r),
    GL_ATTR(SDL_GL_GREEN_SIZE,            colourDepth.g),
    GL_ATTR(SDL_GL_BLUE_SIZE,             colourDepth.b),
    GL_ATTR(SDL_GL_ALPHA_SIZE,            colourDepth.a),
    GL_ATTR(SDL_GL_DEPTH_SIZE,            depthBufferSize),
    GL_ATTR(SDL_GL_STENCIL_SIZE,          stencilBufferSize),
    GL_ATTR(SDL_GL_MULTISAMPLEBUFFERS,    multiSampling ? 1 : 0),
    GL_ATTR(SDL_GL_MULTISAMPLESAMPLES,    multiSamplingBuffers),
    GL_ATTR(SDL_GL_DOUBLEBUFFER,          1)
  };
  const unsigned int attrCount = sizeof(attrs) / sizeof(attrs[0]);
for (unsigned int i = 0; i < attrCount; ++i)
  {
    SDL_GL_SetAttribute(attrs[i].id, attrs[i].preferredValue);
  }

  game.window = Window::Create(game.device->GetBackbufferSize(), game.Name);
  if (!game.window)
  {
    return false;
  }

  game.window->SetCurrent();
  for (unsigned int i = 0; i < attrCount; ++i)
  {
    SDL_GL_GetAttribute(attrs[i].id, &attrs[i].actualValue);
    LOG("%s = %d (pref:%d)\n", attrs[i].name, attrs[i].actualValue, attrs[i].preferredValue);
  }

  if (!gl::sys::LoadFunctions())
  {
    return false;
  }
  LOG("GL version: %d.%d\n", gl::sys::GetMajorVersion(), gl::sys::GetMinorVersion());

  // Keep this list in sync with Scene::SceneState and renderer\shader\autouniforms\list.h...
  AddAutoUniformFactory(boost::make_shared<AutoUniform::CameraPositionFactory>());
  AddAutoUniformFactory(boost::make_shared<AutoUniform::ModelMatrixFactory>());
  AddAutoUniformFactory(boost::make_shared<AutoUniform::ViewMatrixFactory>());
  AddAutoUniformFactory(boost::make_shared<AutoUniform::ProjectionMatrixFactory>());
  AddAutoUniformFactory(boost::make_shared<AutoUniform::ViewProjectionMatrixFactory>());
  AddAutoUniformFactory(boost::make_shared<AutoUniform::ModelViewProjectionMatrixFactory>());

  return true;
}

//------------------------------------------------------------

void Device::AddAutoUniformFactory(AutoUniform::Factory::Ptr factory)
{
  autoUniformFactories[factory->GetName()] = factory;
  LOG("GLSL auto-uniform: %s\n", factory->GetName().c_str());
}

//------------------------------------------------------------

void Device::Clear(const ClearState& state)
{
  ApplyColourMask(state.colourMask, pipelineState.colourMask);

  if (state.colour != clearState.colour)
  {
    gl::ClearColor(state.colour.r, state.colour.g, state.colour.b, state.colour.a); 
    clearState.colour = state.colour;
  }

  GLbitfield mask = 0;
  if (state.buffers & Buffers::Colour) { mask |= gl::COLOR_BUFFER_BIT; }
  if (state.buffers & Buffers::Depth) { mask |= gl::DEPTH_BUFFER_BIT; }
  if (state.buffers & Buffers::Stencil) { mask |= gl::STENCIL_BUFFER_BIT; }
  gl::Clear(mask);
}

//------------------------------------------------------------

void Device::Draw(const Renderer::RenderState& newRenderState)
{
  ApplyRenderState(newRenderState, renderState);
}

//------------------------------------------------------------

unsigned int Device::MaxVertexAttributes()
{
  static unsigned int maxVertexAttributes = 0;
  if (!maxVertexAttributes)
  {
    gl::GetIntegerv(gl::MAX_VERTEX_ATTRIBS, (GLint*)&maxVertexAttributes);
    LOG("gl::MAX_VERTEX_ATTRIBS = %d\n", maxVertexAttributes);
  }
  return maxVertexAttributes;
}
