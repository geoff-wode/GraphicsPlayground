#include <Kandy\core\input\keyboardstate.h>

using namespace Kandy::Core;

//-----------------------------------------------

bool KeyboardState::IsKeyDown(KeyCode::Enum key) const
{
  // If the key's in the set, it's pressed...
  return pressedKeys.find(key) != pressedKeys.end();
}

//-----------------------------------------------

bool KeyboardState::IsKeyUp(KeyCode::Enum key) const
{
  // If the key's not in the set, it isn't pressed...
  return pressedKeys.find(key) == pressedKeys.end();
}
