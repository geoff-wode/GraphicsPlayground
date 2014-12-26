#pragma once

#include <Kandy\renderer\shaders\autouniforms\autoshaderuniform.h>
#include <Kandy\renderer\shaders\uniforms\alltypes.h>
#include <Kandy\scene\scenestate.h>

#define AUTO_SHADER_UNIFORM(data_name, data_type, data_source) \
  class data_name##AutoShaderUniform : public AutoShaderUniform \
  { \
  public: \
    data_name##AutoShaderUniform(ShaderUniformBase* const uniform) \
      : uniform((ShaderUniform<data_type>*)uniform) \
    { } \
    void Update(const RenderState& renderState, const Scene::SceneState& sceneState) { uniform->Set(data_source); } \
  private: \
    ShaderUniform<data_type>* const uniform; \
  }; \
  class data_name##AutoShaderUniformFactory : public AutoShaderUniformFactory \
  { \
  public: \
    const char* const Name; \
    data_name##AutoShaderUniformFactory() \
      : Name(#data_name) \
    { } \
    boost::shared_ptr<AutoShaderUniform> Create(ShaderUniformBase* const uniform) \
    { \
      return boost::make_shared<data_name##AutoShaderUniform>(uniform); \
    } \
  };

namespace Kandy
{
  namespace Renderer
  {
#include <Kandy\renderer\shaders\autouniforms\all.h>
  }
}
