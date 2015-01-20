#pragma once

#include <SDL.h>
#include <Kandy\renderer\context.h>
#include "windowimpl.h"

namespace Kandy
{
  namespace Renderer
  {
    struct ContextImpl : public Context
    {
      WindowImpl* const parentWindow;
      SDL_GLContext glContext;
      RenderState   currentRenderState;
      ClearState    currentClearState;
      Core::Rectangle viewport;

      ContextImpl(WindowImpl* const parentWindow);
      virtual ~ContextImpl();
      
      virtual void SetViewport(const Core::Rectangle& rectangle);
      virtual Core::Rectangle GetViewport() const;

      virtual void MakeCurrent();
      virtual Window& GetParentWindow() const;
      virtual void Clear(const ClearState& clearState);
      virtual void Draw(const RenderState& drawState, const SceneState& sceneState);
    };
  }
}
