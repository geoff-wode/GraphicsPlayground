#pragma once

#include <SDL.h>
#include <Kandy\renderer\context.h>
#include <Kandy\renderer\renderstate.h>

namespace Kandy
{
  namespace Renderer
  {
    struct ContextImpl : public Context
    {
      struct WindowImpl* const parentWindow;
      SDL_GLContext glContext;
      RenderState   renderState;

      ContextImpl(struct WindowImpl* const parentWindow);
      virtual ~ContextImpl();
      
      virtual void MakeCurrent();
      virtual Window& GetParentWindow() const;
      virtual void Clear(const ClearState& clearState);
      virtual void Draw(const RenderState& drawState, const SceneState& sceneState);
    };
  }
}
