#pragma once

/**
  @file kandy.h

  Declares the main entry point and main class for applications.
  */

#include <Kandy\core\application.h>

namespace Kandy
{
  /**
    @brief The main entry point to the application.
    
    This function is called early in the application startup sequence, whereup user code
    should instantiate a derived instance of @see Application and call its @see Run method.
    */
  void Main(int argc, char* argv[]);
}
