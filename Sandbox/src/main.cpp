#include <boost\shared_ptr.hpp>
#include <Kandy\kandy.h>
#include <Kandy\core\logging.h>
#include <Kandy\renderer\device.h>
#include <Kandy\renderer\shaders\shader.h>

using namespace Kandy::Core;
using namespace Kandy::Renderer;

//------------------------------------------------------------

class MyApp : public Application
{
public:
  boost::shared_ptr<Context> mainContext;
  ClearState clearState;

  MyApp() 
    : Application()
  {
    INFO("started\n");
  
    clearState.colour = glm::vec4(1,0,0,1);
    clearState.buffers = ClearState::Buffers::ColourDepth;
  }

  virtual void Initialise()
  {
    Device::Initialise();

    mainContext = Device::GetContext();
  }

  virtual void Render(double frameInterpolation)
  {
    mainContext->Clear(clearState);
  }
};

//---------------------------------------------------------------

void Kandy::Main(int argc, char* argv[])
{
  MyApp app;
  app.Run();
}
