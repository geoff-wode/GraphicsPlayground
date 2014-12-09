#pragma once

#include <glm\glm.hpp>
#include <boost\noncopyable.hpp>
#include <Kandy\renderer\clearstate.h>
#include <Kandy\renderer\pipelinestate.h>
#include <Kandy\renderer\renderstate.h>
#include <Kandy\renderer\shader\autouniform.h>

namespace Kandy
{
  namespace Core
  {
    class Game;
  }

  namespace Renderer
  {
    class Device : public boost::noncopyable
    {
    public:
      friend class Core::Game;

      static AutoUniform::FactoryCollection& GetAutoUniformFactories() { return autoUniformFactories; }

      Device();
      ~Device();

      void SetBackbufferSize(const glm::ivec2& value) { backbufferSize = value; }
      glm::ivec2 GetBackbufferSize() const { return backbufferSize; }

      void SetColourDepth(const glm::ivec4& value) { colourDepth = value; }
      glm::ivec4 GetColourDepth() const { return colourDepth; }

      void SetFullScreen(bool value) { fullScreen = value; }
      bool GetFullScreen() const { return fullScreen; }

      void SetMultiSampling(bool value) { multiSampling = value; }
      bool GetMultiSampling() const { return multiSampling; }

      void SetMultiSamplingBuffers(unsigned int value) { multiSamplingBuffers = value; }
      unsigned int GetMultiSamplingBuffers() const { return multiSamplingBuffers; }

      void Clear(const ClearState& state);

      void Draw(const RenderState& state);

      void AddAutoUniformFactory(AutoUniform::Factory::Ptr factory);

      static unsigned int MaxVertexAttributes();

    private:
      bool Initialise(Core::Game& game);

      glm::ivec4 colourDepth;
      glm::ivec2 backbufferSize;
      bool  fullScreen;
      bool  multiSampling;
      unsigned int multiSamplingBuffers;
      unsigned int depthBufferSize;
      unsigned int stencilBufferSize;

      ClearState clearState;
      PipelineState pipelineState;

      static AutoUniform::FactoryCollection autoUniformFactories;
    };
  }
}
