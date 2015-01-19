#pragma once

#include <boost\shared_ptr.hpp>

/**
  @file object.h

  Base class.
  */

namespace Kandy
{
  struct Object
  {
    virtual ~Object() = 0;
  };
  Object::~Object() { } // Define this so all other derivations do not have to.

  typedef boost::shared_ptr<Object> ObjectPtr;
}
