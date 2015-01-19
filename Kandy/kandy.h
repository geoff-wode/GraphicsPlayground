#pragma once

/**
  @file kandy.h

  Declares the main entry point and main class for applications.
  */

namespace Kandy
{
  /**
    @brief The main entry point to the application.
    
    Kandy will call this function early in the application startup sequence.
    */
  void Main(int argc, char* argv[]);

  namespace Core
  {
    /**
      @brief The main base type for all Kandy applications.

      Override the various virtual methods to define application-specific behaviour.
      */
    class Application
    {
    public:
      virtual ~Application();

      /**
        @brief Start execution of the application's main processing loop.
        */
      void Run();

      /**
        @brief Cause the application to terminate.
        */
      void Exit();

    protected:
      Application();

      /**
        @brief Perform one-time application initialisation.

        This method is called before the main processing loop is entered.
        */
      virtual void Initialise();

      /**
        @brief Notifies the application that the main processing loop has been terminated.
        */
      virtual void OnExit();

    private:
      struct Impl;
      Impl* const impl;
    };
  }
}
