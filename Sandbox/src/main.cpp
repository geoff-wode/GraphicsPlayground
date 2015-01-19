#include <boost\shared_ptr.hpp>
#include <Kandy\kandy.h>
#include <Kandy\core\logging.h>
#include <Kandy\renderer\device.h>

using namespace Kandy::Core;
using namespace Kandy::Renderer;

//------------------------------------------------------------

class MyApp : public Application
{
public:
  boost::shared_ptr<Window> mainWindow;
  boost::shared_ptr<Context> mainContext;

  MyApp() 
    : Application()
  {
    INFO("started\n");
  }

  virtual void Initialise()
  {
    Device::Initialise();

    mainWindow = Device::NewWindow(1280, 720);
    mainContext = mainWindow->GetContext();
  }
};

//---------------------------------------------------------------

void Kandy::Main(int argc, char* argv[])
{
  MyApp app;
  app.Run();
}
