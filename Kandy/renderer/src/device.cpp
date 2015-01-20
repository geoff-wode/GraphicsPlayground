#include <exception>
#include <cstdlib>
#include <cstring>
#include <SDL.h>
#include <boost\make_shared.hpp>
#include <Kandy\renderer\device.h>
#include <Kandy\core\logging.h>
#include "shaders\shaderimpl.h"
#include "contextimpl.h"
#include "windowimpl.h"
#include "gl_core_3_3.hpp"

using namespace Kandy::Renderer;

//------------------------------------------------------------

namespace
{
  Device::Configuration config;
  boost::shared_ptr<WindowImpl> window;
  boost::shared_ptr<ContextImpl> context;

  void Shutdown()
  {
    SDL_Quit();
  }
}

//-------------------------------------------------------------------

void Device::Configure(const Device::Configuration& config)
{
  ::config = config;
}

//-------------------------------------------------------------------

bool Device::Initialise()
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
  {
    FATAL("cannot initialise system - %s\n", SDL_GetError());
    return false;
  }
  atexit(Shutdown);

  SDL_version version;
  SDL_GetVersion(&version);
  INFO("SDL v%d.%d.%d\n", version.major, version.minor, version.patch);

  try
  {
    window = boost::make_shared<WindowImpl>(config.backbufferWidth, config.backbufferHeight, config.windowMode);
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    context = boost::make_shared<ContextImpl>(window.get());
  }
  catch (const std::logic_error& e)
  {
    return false;
  }
  return true;
}

//-------------------------------------------------------------------

boost::shared_ptr<Window> Device::GetWindow()
{
  return window;
}

//-------------------------------------------------------------------

boost::shared_ptr<Context> Device::GetContext()
{
  return context;
}

//-------------------------------------------------------------------

boost::shared_ptr<Context> NewContext()
{
  boost::shared_ptr<ContextImpl> context(new ContextImpl(window.get()));

  return context;
}

//-------------------------------------------------------------------

boost::shared_ptr<Shader> Device::NewShader(
  const char* const vertexShaderSource,
  const char* const fragmentShaderSource)
{
  boost::shared_ptr<ShaderImpl> shader;
  try
  {
    shader = boost::make_shared<ShaderImpl>(vertexShaderSource, fragmentShaderSource);
  } catch (const std::logic_error& e)
  {
    FATAL("cannot create shader: %s\n", e.what());
    shader.reset();
  }
  return shader;
}

//-------------------------------------------------------------------

size_t Device::MaxTextureUnits()
{
  GLint value;
  gl::GetIntegerv(gl::MAX_COMBINED_TEXTURE_IMAGE_UNITS, &value);
  return value;
}

//-------------------------------------------------------------------

void Device::SwapBuffers()
{
  window->Swap();
}
