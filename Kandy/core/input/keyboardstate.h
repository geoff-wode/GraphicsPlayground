#pragma once

#include <set>
#include <Kandy\core\input\keycode.h>

namespace Kandy
{
  namespace Core
  {
    /**
      @brief The state of the keyboard when @see Keyboard::GetState is called.
      */
    struct KeyboardState
    {
      KeyboardState() { }
      KeyboardState(const std::set<KeyCode::Enum>& pressedKeys) : pressedKeys(pressedKeys) { }
      KeyboardState(const KeyboardState& other) : pressedKeys(other.pressedKeys) { }
      KeyboardState& operator=(const KeyboardState& other) { pressedKeys = other.pressedKeys; return *this; }

      /**
        @brief The set of keys which are in the pressed state.
        */
      const std::set<KeyCode::Enum>& GetPressedKeys() const { return pressedKeys; }

      bool IsKeyDown(KeyCode::Enum key) const;
      bool IsKeyUp(KeyCode::Enum key) const;

    private:
      std::set<KeyCode::Enum> pressedKeys;
    };
  }
}
