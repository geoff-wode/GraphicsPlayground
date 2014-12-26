#pragma once

#include <string>
#include <boost\shared_ptr.hpp>
#include <boost\make_shared.hpp>
#include <Kandy\renderer\shaders\uniforms\shaderuniformbase.h>

namespace Kandy
{
  namespace Scene
  {
    struct SceneState;
  }

  namespace Renderer
  {
    struct RenderState;

    class AutoShaderUniform
    {
    public:
      virtual ~AutoShaderUniform() { }

      virtual void Update(const RenderState& renderState, const Scene::SceneState& sceneState) = 0;
    };

    class AutoShaderUniformFactory
    {
    public:
      virtual boost::shared_ptr<AutoShaderUniform> Create(ShaderUniformBase* const uniform) = 0;
    };
  }
}
