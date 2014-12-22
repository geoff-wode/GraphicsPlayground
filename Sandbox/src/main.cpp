#include <Kandy\core\program.h>
#include <Kandy\core\game.h>
#include <Kandy\core\logging.h>
#include <Kandy\renderer\device.h>
#include <Kandy\renderer\clearstate.h>

using namespace Kandy::Core;
using namespace Kandy::Renderer;

class MyGame : public Kandy::Core::Game
{
public:
  const double delayMs;
  double time;
  int selector;
  ClearState clearState[2];

  MyGame() 
    : Kandy::Core::Game(),
      delayMs(1000 * 5)
  {
  }

  void Initialise()
  {
    Game::Initialise();

    time = 0;
    selector = 0;
    clearState[0].colourBuffer.colour = glm::vec4(1, 0, 0, 1);
    clearState[1].colourBuffer.colour = glm::vec4(0, 0, 1, 1);
  }

  void Update(double elapsedMs)
  {
    time += elapsedMs;
    if (time > delayMs)
    {
      time -= delayMs;
      selector ^= 1;
    }

    Game::Update(elapsedMs);

    //Exit();
  }

  void Render(double elapsedMs)
  {
    Device->Clear(ClearState::Buffers::Colour, clearState[selector]);
    Game::Render(elapsedMs);
  }
};

//---------------------------------------------------------------

void Kandy::Core::Program::Main(int argc, char* argv[])
{
  MyGame game;
  game.Run();
}
