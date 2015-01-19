#pragma once

/**
  @file logging.h

  Contains various helpers for logging notable application events to file.
  */

#include <cassert>
#include <cstdarg>
#include <cstdio>

#define KANDY_LOG_TO_STR_HELPER(v) #v
#define KANDY_LOG_TO_STR(v) KANDY_LOG_TO_STR_HELPER(v)
#define KANDY_LOG(level, msg, ...)  Kandy::Core::Logger::Log(Kandy::Core::Logger::Level::##level, __FILE__ ":" KANDY_LOG_TO_STR(__LINE__) ":" #level ":" msg, __VA_ARGS__)

/**
  @brief Report a message of application-ending severity.
  */
#define FATAL(msg, ...)             KANDY_LOG(Fatal, msg, __VA_ARGS__)

/**
  @brief Report a message of worrying severity.
  */
#define WARNING(msg, ...)           KANDY_LOG(Warning, msg, __VA_ARGS__)

/**
  @brief Report a message of noteworthiness.
  */
#define INFO(msg, ...)              KANDY_LOG(Info, msg, __VA_ARGS__)

/**
  @brief Ensure that a runtime-evaluated condition is true, aborting application execution if it is not.
  @param cond   Boolean condition which must be true.
  */
#define ASSERT(cond) \
  do \
  { \
    if (!(cond)) \
    { \
      FATAL("ASSERT: %s\n", #cond); \
      assert(false); \
    } \
  } while (0)

namespace Kandy
{
  namespace Core
  {
    namespace Logger
    {
      /**
        @brief Logging severity mask.

        Logging can be filtered by severity at runtime, such that lower severity messages
        are not recorded.
        */
      struct Level
      {
        enum Enum
        {
          None,
          Fatal,
          Warning,
          Info
        };
      };

      void SetLevel(Level::Enum level);
      Level::Enum GetLevel();

      void Log(Level::Enum level, const char* const fmt, ...);
    };
  }
}
