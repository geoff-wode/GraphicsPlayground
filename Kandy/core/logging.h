#pragma once

#include <assert.h>
#include <stdarg.h>

#define KANDY_LOG_TO_STR_HELPER(v) #v
#define KANDY_LOG_TO_STR(v) KANDY_LOG_TO_STR_HELPER(v)
#define KANDY_LOG(level, msg, ...)  Kandy::Core::Logger::Log(Kandy::Core::Logger::Level::##level, __FUNCTION__ ":" KANDY_LOG_TO_STR(__LINE__) ":" #level ":" msg, __VA_ARGS__)

#define FATAL(msg, ...)             KANDY_LOG(Fatal, msg, __VA_ARGS__)
#define WARNING(msg, ...)           KANDY_LOG(Warning, msg, __VA_ARGS__)
#define INFO(msg, ...)              KANDY_LOG(Info, msg, __VA_ARGS__)

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
      namespace Level
      {
        enum Enum
        {
          None    = 0,
          Fatal   = 1,
          Warning = 2,
          Info    = 4,
          All = Fatal | Warning | Info
        };
      }

      void SetLevel(Level::Enum level);
      Level::Enum GetLevel();

      void Log(Level::Enum level, const char* const fmt, ...);
    }
  }
}
