#include <Kandy\core\program.h>
#include <Kandy\core\game.h>
#include <Kandy\core\logging.h>
#include <Kandy\renderer\clearstate.h>
#include <Kandy\core\resourceloader.h>
#include <Kandy\renderer\shader\autouniform.h>
#include <Kandy\renderer\shader\uniformtypes\all.h>

#include <resource.h>
#include <string>

class MyGame : public Kandy::Core::Game
{
public:
  Kandy::Renderer::ClearState redOrBlue[2];
  unsigned int selector;
  static const unsigned int delay = 5000;
  unsigned int timer;

  MyGame()
    : Game("Sandbox")
  {
    Kandy::Core::ResourceLoader::Resource res;
    res.id = IDR_TEXTFILE2;
    res.type = TEXTFILE;
    Kandy::Core::ResourceLoader::Load(res);
    const std::string s(res.data, res.size);
    LOG("I loaded \"%s\"!\n", s.c_str());
  }

  virtual void Initialise()
  {
    Game::Initialise();

    redOrBlue[0].colour = glm::vec4(1,0,0,1);
    redOrBlue[1].colour = glm::vec4(0,0,1,1);
    selector = 0;
    timer = 0;
  }

  virtual void Update(unsigned int elapsedMs)
  {
    timer += elapsedMs;
    if (timer >= delay)
    {
      timer -= delay;
      selector ^= 1;
    }
    return Game::Update(elapsedMs);
  }

  virtual void Render(unsigned int elapsedMs)
  {
    device->Clear(redOrBlue[selector]);

    Game::Render(elapsedMs);
  }
};

void Kandy::Core::Program::Main(int argc, char* argv[])
{
  MyGame game;
  game.Run();
}
