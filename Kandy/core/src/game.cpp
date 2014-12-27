#include <SDL.h>
#include <core\game.h>
#include <core\logging.h>
#include "keyboardinternal.h"
#include <renderer\device.h>
#include <boost\foreach.hpp>
#include <boost\scoped_ptr.hpp>
#include <boost\make_shared.hpp>

using namespace Kandy;
using namespace Kandy::Core;

//---------------------------------------------------------

struct Game::Impl
{
  Impl()
    : running(true)
  {
  }

  bool running;
  Renderer::Device device;
};

//---------------------------------------------------------

static const double fps = 60.0;
static const double msPerFrame = 1000.0 / fps;

static void GetSDLEvents(Game* game);
static void HandleWindowEvent(Game* game, SDL_Event* event);

//---------------------------------------------------------

Game::Game()
  : impl(new Impl())
{
  Device = &impl->device;
}

//---------------------------------------------------------

Game::~Game()
{
  delete impl;
}

//---------------------------------------------------------

void Game::Run()
{
  Initialise();

  double lag = 0.0;
  double prevTime = 0.0;

  while (impl->running)
  {
    GetSDLEvents(this);
    if (impl->running)
    {
      // Work out how much real time has passed sine the last update...
      const double now = SDL_GetTicks();
      const double elapsed = now - prevTime;
      prevTime = now;
      lag += elapsed;

      // Update the simulation. If several frames of state change have
      // been missed, make repeated calls to catch up...
      while (impl->running && (lag > msPerFrame))
      {
        Update(msPerFrame);
        lag -= msPerFrame;
      }

      if (impl->running)
      {
        // Render the scene.
        // Linearly interpolate the visible state so that rendering can
        // take place between the time steps of the simulation.
        const double framePosition = lag / msPerFrame;
        Render(framePosition);

        Device->PresentBackbuffer();
      }

      SDL_Delay(1);
    }
  }
}

//---------------------------------------------------------

void Game::Exit()
{
  impl->running = false;
}

//---------------------------------------------------------

void Game::Initialise()
{
  Device->Initialise(this);
  Keyboard::Initialise();
}

//---------------------------------------------------------

void Game::Update(double elapsedMs)
{
}

//---------------------------------------------------------

void Game::Render(double elapsedMs)
{
}

//---------------------------------------------------------

static void GetSDLEvents(Game* game)
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:        game->Exit(); break;
    case SDL_WINDOWEVENT: HandleWindowEvent(game, &event); break;
    case SDL_KEYDOWN:     Keyboard::KeyPressed((KeyCode::Enum)event.key.keysym.scancode); break;
    case SDL_KEYUP:       Keyboard::KeyReleased((KeyCode::Enum)event.key.keysym.scancode); break;
    default: break;
    }
  }
}

//---------------------------------------------------------

static void HandleWindowEvent(Game* game, SDL_Event* event)
{
  switch (event->window.event)
  {
  case SDL_WINDOWEVENT_CLOSE:
    event->type = SDL_QUIT;
    SDL_PushEvent(event);
    break;
  default:
    break;
  }
}
