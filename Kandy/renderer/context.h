#pragma once

/**
  @file context.h

  */

namespace Kandy
{
  namespace Renderer
  {
    struct ClearState;
    struct RenderState;
    struct SceneState;
    struct Window;

    /**
      @brief A context is essentially where rendering calls are made from.
      */
    struct Context
    {
      /**
        Make this context the current destination for rendering operations.
        */
      virtual void MakeCurrent() = 0;

      virtual Window& GetParentWindow() const = 0;

      /**
        Clear the viewport using the provided state.
        */
      virtual void Clear(const ClearState& clearState) = 0;

      /**
        Draw primitives using the provided state.
        */
      virtual void Draw(const RenderState& drawState, const SceneState& sceneState) = 0;

    protected:
      ~Context() { }
    };
  }
}
