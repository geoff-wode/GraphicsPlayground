#pragma once

#include <boost\shared_ptr.hpp>
#include <boost\make_shared.hpp>
#include <Kandy\renderer\shader\autouniform.h>
#include <Kandy\renderer\shader\uniformtypes\all.h>

/*
This macro takes the pain out of defining a new shader automatic uniform.
*/
#define AUTO_UNIFORM_DEFINITION(name, type, source) \
  struct name : public Interface \
  { \
    name(Uniform::Ptr uniform) \
    { \
      this->uniform = (type*)uniform.get(); \
    } \
    virtual void Set(const PipelineState& pipeline, const Scene::SceneState& scene) \
    { \
      uniform->Set(source); \
    } \
  private: \
    type* uniform; \
  }; \
  class name##Factory : public Factory \
  { \
  public: \
  virtual std::string GetName() const { return #name ; } \
    virtual AutoUniform::Ptr Create(Uniform::Ptr uniform) \
    { \
      return boost::make_shared<name>(uniform); \
    } \
  }

namespace Kandy
{
  namespace Renderer
  {
    namespace AutoUniform
    {
      // Keep this list in sync with Scene::SceneState...
      AUTO_UNIFORM_DEFINITION(CameraPosition, UniformFloatVector3, scene.cameraPosition);
      AUTO_UNIFORM_DEFINITION(ModelMatrix, UniformFloatMatrix4x4, scene.modelMatrix);
      AUTO_UNIFORM_DEFINITION(ViewMatrix, UniformFloatMatrix4x4, scene.viewMatrix);
      AUTO_UNIFORM_DEFINITION(ProjectionMatrix, UniformFloatMatrix4x4, scene.projectionMatrix);
      AUTO_UNIFORM_DEFINITION(ViewProjectionMatrix, UniformFloatMatrix4x4, scene.viewProjectionMatrix);
      AUTO_UNIFORM_DEFINITION(ModelViewProjectionMatrix, UniformFloatMatrix4x4, scene.modelViewProjectionMatrix);
    }
  }
}
