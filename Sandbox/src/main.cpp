#include <cstdint>
#include <glm\glm.hpp>
#include <Kandy\core\program.h>
#include <Kandy\core\game.h>
#include <Kandy\core\logging.h>
#include <Kandy\renderer\device.h>
#include <Kandy\renderer\clearstate.h>

#include <boost\shared_ptr.hpp>
#include <boost\make_shared.hpp>

using namespace Kandy::Core;
using namespace Kandy::Renderer;

//------------------------------------------------------------

#pragma pack(push, 1)
struct AVertex
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 textureCoord;
} vertices[4];
#pragma pack(pop)

static VertexArrayAttribute vertexAttrs[] =
{
  { 3, offsetof(AVertex, position), ComponentType::Float, false },
  { 3, offsetof(AVertex, normal), ComponentType::Float, false },
  { 2, offsetof(AVertex, textureCoord), ComponentType::Float, false }
};
static const size_t attrCount = sizeof(vertexAttrs) / sizeof(vertexAttrs[0]);

//------------------------------------------------------------

class MyGame : public Kandy::Core::Game
{
public:
  const double delayMs;
  double time;
  int selector;
  ClearState clearState[2];
  boost::shared_ptr<VertexArray> va;
  boost::shared_ptr<VertexBuffer> vb;
  boost::shared_ptr<IndexBuffer> ib;

  MyGame() 
    : Kandy::Core::Game(),
      delayMs(1000 * 1)
  {
  }

  void Initialise()
  {
    Game::Initialise();

    time = 0;
    selector = 0;
    clearState[0].colourBuffer.colour = glm::vec4(1, 0, 0, 1);
    clearState[1].colourBuffer.colour = glm::vec4(0, 0, 1, 1);

    uint16_t indices[] = { 0, 1, 2 };
    ib = boost::make_shared<IndexBuffer>(IndexBuffer::DataType::UInt16, 3, BufferUsageHint::StaticDraw);
    ib->SetData(indices, 3);

    vb = boost::make_shared<VertexBuffer>(sizeof(vertices), BufferUsageHint::StaticDraw);

    va = boost::make_shared<VertexArray>();
    va->Indices = ib.get();
    va->Vertices = vb.get();
    va->AddAttribute(vertexAttrs[0]);
    va->AddAttribute(vertexAttrs[1]);
    va->AddAttribute(vertexAttrs[2]);
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

    Exit();
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
