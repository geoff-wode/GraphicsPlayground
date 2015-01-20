#include <SDL.h>
#include <Kandy\core\logging.h>
#include <Kandy\core\input\keyboard.h>
#include "keyboardimpl.h"

using namespace Kandy::Core;

//--------------------------------------------------------

namespace
{
  std::set<KeyCode::Enum> pressedKeys;
}

//--------------------------------------------------------

KeyboardState Keyboard::GetState()
{
  return KeyboardState(pressedKeys);
}

//--------------------------------------------------------

void Keyboard::WasPressed(KeyCode::Enum key)
{
  pressedKeys.insert(key);
}

//--------------------------------------------------------

void Keyboard::WasReleased(KeyCode::Enum key)
{
  pressedKeys.erase(key);
}
