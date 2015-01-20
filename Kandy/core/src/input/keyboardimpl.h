#pragma once

#include <Kandy\core\input\keycode.h>

namespace Kandy
{
  namespace Core
  {
    namespace Keyboard
    {
      void WasPressed(KeyCode::Enum key);
      void WasReleased(KeyCode::Enum key);
    }
  }
}
