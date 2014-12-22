#pragma once

#include <Kandy\core\game.h>
#include <Kandy\renderer\devicecontext.h>
#include <Kandy\renderer\clearstate.h>
#include <Kandy\renderer\renderstate.h>

namespace Kandy
{
  namespace Renderer
  {
    class Device
    {
    public:
      Device();
      virtual ~Device();

      // Call this before Game::Initialise is called to configure the display attributes
      // of the main window.
      // The context cannot be modified after Device::Initialise is called.
      void SetContext(const DeviceContext& context);

      // Present the game display as full screen or in a window.
      void SetFullScreen(bool enable);

      void Clear(const ClearState::Buffers::Enum& buffers, const ClearState& state);

      // When specifying an index buffer, count specifies the number of indices to process.
      // Otherwise count specifies the number of _vertices_ to process.
      void Render(PrimitiveType::Enum primitive, int count, const RenderState& renderState);

      // When specifying an index buffer, count specifies the number of indices to process and offset is
      // a position within the index buffer to start from.
      // Otherwise count specifies the number of _vertices_ to process and offset is a position within
      // the vertex stream to start from.
      void Render(PrimitiveType::Enum primitive, int count, int offset, const RenderState& renderState);

#pragma region Internal
      // Called by Game::Initialise before the main update loop is entered.
      void Initialise(Core::Game* const game);

      // If double-buffering is defined by the DeviceContext, called after rendering is
      // completed in order to flip the front- and backbuffers.
      void PresentBackbuffer();
#pragma endregion Internal

    private:
      struct Impl;
      Impl* impl;
    };
  }
}
