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

      /**
        @brief Override this method to update internal logic.
        */
      virtual void Update(double elapsedMilliseconds);

      /**
        @brief Override this method to prevent rendering, e.g. if the application minimised.
        @return true if rendering should proceed, false otherwise.
        */
      virtual bool CanRender();

      /**
        @breif Override this method to perform rendering actions.

        Render is only called if @see CanRender returns true.

        @param frameInterpolation The "distance" the simulation has moved into the next
                                  frame, enabling interpolation of motion when rendering
                                  lags behind @see Update. Values are in the range [0..1]
                                  with 0 representing the start of the /current/ frame
                                  and 1 the end.
        */
      virtual void Render(double frameInterpolation);

      /**
        @breif Override this method to perform any post-rendering actions, such as resource clean-up.

        PostRender is only called if @see CanRender returns true.
        */
      virtual void PostRender();

    private:
      struct Impl;
      Impl* const impl;
    };
  }
}
