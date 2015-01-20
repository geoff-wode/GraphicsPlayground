/**
  @file

  Main entry point to the Kandy subsystem.
  */

#include <exception>
#include <cstdlib>
#include <string>
#include <vector>
#include <SDL.h>
#include <boost\foreach.hpp>
#include <Kandy\kandy.h>
#include "input\keyboardimpl.h"
#include <Kandy\core\logging.h>
#include <Kandy\renderer\device.h>

using namespace Kandy;
using namespace Kandy::Core;
using namespace Kandy::Renderer;

//------------------------------------------------------------

struct Application::Impl
{
  Impl()
    : running(true),
      wantsToExit(false)
  {
  }
  bool running;
  bool wantsToExit;
};

//------------------------------------------------------------

namespace
{
  void OnWindowClosing()
  {
    SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);
  }

  //------------------------------------------------------------

  void HandleWindowEvent(Application* const app, SDL_WindowEvent& event)
  {
    switch (event.event)
    {
    case SDL_WINDOWEVENT_CLOSE: OnWindowClosing(); break;
    default: break;
    }
  }

  //------------------------------------------------------------

  void PollEvents(Application* const app)
  {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT:        app->Exit(); break;
      case SDL_WINDOWEVENT: HandleWindowEvent(app, event.window); break;
      case SDL_KEYDOWN:     Keyboard::WasPressed((KeyCode::Enum)event.key.keysym.scancode); break;
      case SDL_KEYUP:       Keyboard::WasReleased((KeyCode::Enum)event.key.keysym.scancode); break;
      default: break;
      }
    }
  }
}

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
  if (!Device::Initialise())
  {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", "Cannot start application - refer to log for details", NULL);
    return;
  }

  Initialise();

  // Enter the loop...

  // Note that targetFramesPerSecond is _not_ the display rate! It's the length
  // of one "tick" of the application's simulation clock.
  const double targetFramesPerSecond = 120;
  const double millisecondsPerFrame = targetFramesPerSecond / 1000;
  double previousTime = SDL_GetTicks();
  double lag = 0.0;
  while (impl->running)
  {
    PollEvents(this);

    // An input event can result in the app being asked to close down.
    // Allow the application to make a final judgement on whether that actually happens
    // but do it outside of the event polling loop...
    if (impl->wantsToExit)
    {
      if (AllowExit())
      {
        impl->running = false;
        continue; // jump back to the start of the loop, avoiding update and render calls.
      }
      else
      {
        impl->wantsToExit = false;
      }
    }

    // Now make the game "catch up" to wall-clock "now" in a series of
    // discrete time steps, each of a fixed size...
    const double now = SDL_GetTicks();
    const double elapsedMilliseconds = now - previousTime;
    previousTime = now;
    lag += elapsedMilliseconds;
    while (lag >= millisecondsPerFrame)
    {
      // Update involves calling out the user application, whereupon it
      // will go through the process of jumping through all the logical
      // hoops it needs to in order to provide some semblance of an
      // actual interactive experience to the schmuck sitting in front of
      // the screen.
      Update(millisecondsPerFrame);

      // Dealt with that timeslice. Are there more "ticks" of simulation to catch up with..?
      lag -= millisecondsPerFrame;
    }

    // Rendering involves calling out to the user application, giving it
    // the option to circumvent all rendering if it so desires.
    if (CanRender())
    {
      Render(lag / millisecondsPerFrame);
      PostRender();
    }
    Device::SwapBuffers();

    // Yield CPU...
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
  impl->wantsToExit = true;
}

//------------------------------------------------------------

void Application::OnExit()
{
}

//------------------------------------------------------------

bool Application::AllowExit()
{
  return true;
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

