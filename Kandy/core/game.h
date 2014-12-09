#pragma once

#include <boost\noncopyable.hpp>
#include <boost\shared_ptr.hpp>
#include <Kandy\renderer\device.h>
#include <Kandy\renderer\window.h>

namespace Kandy
{
  namespace Core
  {
    class Game : public boost::noncopyable
    {
    public:
      virtual ~Game();

      void Run();
      void Exit();

      const char* const Name;
      boost::shared_ptr<Renderer::Device> device;
      boost::shared_ptr<Renderer::Window> window;

    protected:
      Game(const char* const name);
      virtual void Initialise();
      virtual void Update(unsigned int elapsedMs);
      virtual bool PreRender(unsigned int elapsedMs);
      virtual void Render(unsigned int elapsedMs);
      virtual void PostRender(unsigned int elapsedMs);

    private:
      bool running;
      unsigned int lastUpdate;
      unsigned int lastRender;
      const unsigned int frameRate;
    };
  }
}
