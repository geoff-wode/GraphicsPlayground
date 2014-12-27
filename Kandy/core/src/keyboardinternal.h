#pragma once

#include <core\input\keyboard.h>

namespace Kandy
{
  namespace Core
  {
    namespace Keyboard
    {
      void Initialise();
      void KeyPressed(KeyCode::Enum key);
      void KeyReleased(KeyCode::Enum key);
    }
  }
}
