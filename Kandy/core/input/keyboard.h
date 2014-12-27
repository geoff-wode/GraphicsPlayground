#pragma once

#include <set>
#include <Kandy\core\input\keycode.h>

namespace Kandy
{
  namespace Core
  {
    class KeyboardState
    {
    public:
      KeyboardState() { }
      KeyboardState(const std::set<KeyCode::Enum> pressedKeys) : PressedKeys(pressedKeys) { }
      KeyboardState(const KeyboardState& other) : PressedKeys(other.PressedKeys) { }
      KeyboardState(const KeyboardState&& other) : PressedKeys(other.PressedKeys) { }
      KeyboardState& operator=(const KeyboardState& other) { PressedKeys = other.PressedKeys; return *this; }

      bool IsKeyDown(KeyCode::Enum key) const { return PressedKeys.find(key) != PressedKeys.end(); }
      bool IsKeyUp(KeyCode::Enum key) const { return !IsKeyDown(key); }

      std::set<KeyCode::Enum> PressedKeys;
    };

    namespace Keyboard  // why faff around with a singleton class
    {
      KeyboardState GetState();
    }
  }
}
