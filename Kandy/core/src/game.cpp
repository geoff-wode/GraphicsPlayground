#include <SDL.h>
#include <core\game.h>
#include <core\logging.h>

using namespace Kandy;
using namespace Kandy::Core;
using namespace Kandy::Renderer;

//---------------------------------------------------------

static void GetSDLEvents(Game* game);
static void HandleWindowEvent(Game* game, SDL_Event* event);

//---------------------------------------------------------

Game::Game(const char* const name)
  : Name(name),
    running(true),
    lastUpdate(0),
    lastRender(0),
    frameRate(1000/60),
    device(new Renderer::Device())
{
}

Game::~Game()
{
}

void Game::Run()
{
  Initialise();
  while (running)
  {
    GetSDLEvents(this);
    if (running)
    {
      const unsigned int now = SDL_GetTicks();
      const unsigned int elapsedMs = now - lastUpdate;
      Update(elapsedMs);
      lastUpdate = now;
      if ( ((now - lastRender) >= frameRate) && PreRender(elapsedMs) )
      {
        Render(elapsedMs);
        PostRender(elapsedMs);
        lastRender = now;
      }
      SDL_Delay(1);
    }
  }
}

void Game::Exit()
{
  running = false;
}

void Game::Initialise()
{
  if (!device->Initialise(*this))
  {
    Exit();
    return;
  }
  window->SetCurrent();
  window->Show();
}

void Game::Update(unsigned int elapsedMs)
{
}

bool Game::PreRender(unsigned int elapsedMs)
{
  return true;
}

void Game::Render(unsigned int elapsedMs)
{
}

void Game::PostRender(unsigned int elapsedMs)
{
  window->SwapBuffers();
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

//---------------------------------------------------------
