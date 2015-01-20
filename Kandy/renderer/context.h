#pragma once

#include <Kandy\core\rectangle.h>
#include <Kandy\renderer\clearstate.h>
#include <Kandy\renderer\renderstate.h>
#include <Kandy\renderer\window.h>

/**
  @file context.h

  */

namespace Kandy
{
  namespace Renderer
  {
    struct SceneState;

    /**
      @brief A context is essentially where rendering calls are made from.
      */
    struct Context
    {
      /**
        @brief Make this context the current destination for rendering operations.

        Note that this is a <i>very</i> expensive operation.
        */
      virtual void MakeCurrent() = 0;

      /**
        Get a reference to the parent window which houses this context.
        */
      virtual Window& GetParentWindow() const = 0;

      virtual void SetViewport(const Core::Rectangle& rectangle) = 0;
      virtual Core::Rectangle GetViewport() const = 0;

      /**
        Clear the viewport using the provided state.
        */
      virtual void Clear(const ClearState& clearState) = 0;

      /**
        Draw primitives using the provided state.
        */
      virtual void Draw(const RenderState& renderState, const SceneState& sceneState) = 0;

    protected:
      ~Context() { }
    };
  }
}
