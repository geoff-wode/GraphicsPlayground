#pragma once

#include <vector>
#include <map>
#include <string>
#include <boost\noncopyable.hpp>
#include <boost\shared_ptr.hpp>
#include <Kandy\renderer\shader\uniform.h>
#include <Kandy\renderer\shader\uniformobserver.h>
#include <Kandy\renderer\shader\autouniform.h>
#include <Kandy\scene\scenestate.h>
#include <Kandy\renderer\drawstate.h>

namespace Kandy
{
  namespace Renderer
  {
    class Shader : public boost::noncopyable, public IUniformObserver
    {
    public:
      typedef boost::shared_ptr<Shader> Ptr;
      static Ptr Create(const char* const vsSrc, const char* const fsSrc);

      ~Shader();

      typedef std::pair<const std::string, Uniform::Ptr> UniformSetPair;
      typedef std::map<const std::string, Uniform::Ptr> UniformSet;
      const UniformSet& Uniforms;

      void Activate();
      void UpdateUniforms();

      virtual void HasChanged(Uniform* uniform);

    private:
      Shader();
      UniformSet  uniforms;
      unsigned int program;
      std::vector<Uniform*> changedUniforms;
      std::vector<AutoUniform::Ptr> automaticUniforms;

      void UpdateAutoUniforms(const PipelineState& pipelineState, const Scene::SceneState& sceneState);
      bool GetUniforms();
    };
  }
}
