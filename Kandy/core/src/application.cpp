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

  while (impl->running)
  {
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
