#pragma once

#include <map>
#include <string>
#include <boost\shared_ptr.hpp>
#include <Kandy\renderer\renderstate.h>
#include <Kandy\scene\scenestate.h>

namespace Kandy
{
  namespace Renderer
  {
    /*
    Automatic uniforms allow shaders to just use various pre-defined uniform parameters like the model-view-projection
    matrix without the application itself having to worry about setting the value for each shader in every frame: they
    simply have the value they need by the time the shader gets to run.

    The Device maintains a list of auto uniforms. This is queried when a shader is created and any uniforms in there
    that match a name in the list will be set by the renderer.

    See the Kandy\renderer\autouniforms directory for the list of uniforms available.

    Note that applications are perfectly capable of defining additional automatic uniforms and adding them to Device's
    list. Make sure to do this in MyGame::Initialise after calling the Game::Initialise base method.
    */

    namespace AutoUniform
    {
      struct Interface
      {
        virtual void Set(const PipelineState& pipelineState, const Scene::SceneState& sceneState) = 0;
      };
      typedef boost::shared_ptr<Interface> Ptr;
    
      class Factory
      {
      public:
        virtual std::string GetName() const = 0;
        virtual AutoUniform::Ptr Create(Uniform::Ptr uniform) = 0;

        typedef boost::shared_ptr<Factory> Ptr;
      };

      typedef std::map<const std::string, Factory::Ptr> FactoryCollection;
    }
  }
}
