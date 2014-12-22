#pragma once

#include <list>

namespace Kandy
{
  namespace Renderer
  {
    class Device;
  }

  namespace Core
  {
    class Game
    {
    public:
      virtual ~Game();

      void Run();
      void Exit();

      Renderer::Device* Device;

    protected:
      Game();
      virtual void Initialise();
      virtual void Update(double elapsedMs);
      virtual void Render(double elapsedMs);

    private:
      struct Impl;
      Impl* impl;
    };
  }
}
