#include <SDL.h>
#include "keyboardinternal.h"
#include <boost\foreach.hpp>
#include <boost\shared_ptr.hpp>
#include <boost\make_shared.hpp>

using namespace Kandy::Core;

//-----------------------------------------------------------

static boost::shared_ptr<std::set<KeyCode::Enum> > keyState;

//-----------------------------------------------------------

void Keyboard::Initialise()
{
  keyState = boost::make_shared<std::set<KeyCode::Enum> >();
}

//-----------------------------------------------------------

void Keyboard::KeyPressed(KeyCode::Enum key) { keyState->insert(key); }

//-----------------------------------------------------------

void Keyboard::KeyReleased(KeyCode::Enum key) { keyState->erase(key); }

//-----------------------------------------------------------

KeyboardState Keyboard::GetState() { return KeyboardState(*keyState); }

//-----------------------------------------------------------
