#pragma once

#include <map>
#include <string>
#include <boost\shared_ptr.hpp>
#include <Kandy\renderer\shaders\shadervertexattribute.h>
#include <Kandy\renderer\shaders\uniforms\shaderuniform.h>
#include <Kandy\renderer\shaders\uniforms\shaderuniformobserver.h>
#include <Kandy\renderer\shaders\autouniforms\define.h>

namespace Kandy
{
  namespace Scene
  {
    struct SceneState;
  }

  namespace Renderer
  {
    struct RenderState;

    class Shader : public ShaderUniformObserver
    {
    public:
      Shader();
      ~Shader();

      bool Create(const char* const vertexShader, const char* const fragmentShader);

      typedef std::map<std::string, boost::shared_ptr<ShaderUniformBase> > ShaderUniforms;
      const ShaderUniforms& GetUniforms() const;

      typedef std::map<std::string, ShaderVertexAttribute> ShaderVertexAttributes;
      const ShaderVertexAttributes& GetVertexAttributes() const;

      int GetFragmentOutput(const std::string& name);

      void Bind();  // NB. No "unbind"! A shader is always required.

      // Send all modified uniform values for this shader to the GPU.
      void Update(const RenderState& renderState, const Scene::SceneState& sceneState);

      virtual void NotifyUniformValueChanged(ShaderUniformBase* uniform);

    private:
      struct Impl;
      Impl* impl;
    };
  }
}
