/**
  @file

  Main entry point to the Kandy subsystem.
  */

#include <exception>
#include <cstdlib>
#include <string>
#include <vector>
#include <Windows.h>
#include <SDL.h>
#include <boost\foreach.hpp>
#include <Kandy\kandy.h>
#include <Kandy\core\logging.h>

using namespace Kandy;
using namespace Kandy::Core;

//------------------------------------------------------------

struct Application::Impl
{
  Impl()
    : running(false)
  {
  }
  bool running;
};


//------------------------------------------------------------

int main(int argc, char* argv[])
{
  Main(argc, argv);

  return 0;
}

//------------------------------------------------------------

Application::Application()
  : impl(new Impl())
{
}

//------------------------------------------------------------

Application::~Application()
{
  delete impl;
}

//------------------------------------------------------------

void Application::Run()
{
  Initialise();

  // Enter the loop...

  // Note that targetFramesPerSecond is _not_ the display rate! It's the size
  // of one "tick" of the application's internal world clock.
  const double targetFramesPerSecond = 120;
  const double millisecondsPerFrame = targetFramesPerSecond / 1000;
  double previousTime = SDL_GetTicks();
  double lag = 0.0;
  while (impl->running)
  {
    const double now = SDL_GetTicks();
    const double elapsedMilliseconds = now - previousTime;
    previousTime = now;
    lag += elapsedMilliseconds;

    // TODO: Handle input

    // Now make the game "catch up" to wall-clock "now" in a series of
    // discrete time steps, each of a fixed size...
    while (lag >= millisecondsPerFrame)
    {
      // Update involves calling out the user application, whereupon it
      // will go through the process of jumping through all the logical
      // hoops it needs to in order to provide some semblance of an
      // actual interactive experience to the schmuck sitting in front of
      // the screen.
      Update(millisecondsPerFrame);

      // Dealt with that timeslice:
      lag -= millisecondsPerFrame;
    }

    // Rendering involves calling out to the user application, giving it
    // the option to circumvent all rendering if it si desires.
    if (CanRender())
    {
      Render(lag / millisecondsPerFrame);
      PostRender();
    }

    // Yield to whatever else is going on in the PC:
    SDL_Delay(1);
  }

  OnExit();
}

//------------------------------------------------------------

void Application::Initialise()
{
}

//------------------------------------------------------------

void Application::Exit()
{
  impl->running = false;
}

//------------------------------------------------------------

void Application::OnExit()
{
}

//------------------------------------------------------------

void Application::Update(double elapsedMilliseconds)
{
}

//------------------------------------------------------------

bool Application::CanRender()
{
  return true;
}

//------------------------------------------------------------

void Application::Render(double frameInterpolation)
{
}

//------------------------------------------------------------

void Application::PostRender()
{
}

//------------------------------------------------------------

