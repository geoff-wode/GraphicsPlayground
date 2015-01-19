#include <exception>
#include <cstdlib>
#include <cstring>
#include <Windows.h>
#include <SDL.h>
#include <boost\make_shared.hpp>
#include <Kandy\renderer\device.h>
#include <Kandy\core\logging.h>
#include "shaders\shaderimpl.h"
#include "windowimpl.h"
#include "gl_core_3_3.hpp"

using namespace Kandy::Renderer;

//------------------------------------------------------------

namespace
{
  void Shutdown()
  {
    SDL_Quit();
  }
}

//-------------------------------------------------------------------

void Device::Initialise()
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
  {
    MessageBox(NULL, SDL_GetError(), "ERROR", MB_OK | MB_ICONERROR | MB_TASKMODAL);
    FATAL("cannot initialise system - %s\n", SDL_GetError());
    throw new std::logic_error(SDL_GetError());
  }
  atexit(Shutdown);

  SDL_version version;
  SDL_GetVersion(&version);
  INFO("SDL v%d.%d.%d\n", version.major, version.minor, version.patch);
}

//-------------------------------------------------------------------

boost::shared_ptr<Window> Device::NewWindow(unsigned int width, unsigned int height)
{
  boost::shared_ptr<WindowImpl> window;
  try
  {
    window = boost::make_shared<WindowImpl>(width, height);
  } catch (const std::logic_error& e)
  {
    FATAL("cannot create window: %s\n", e.what());
    window.reset();
  }
  return window;
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
