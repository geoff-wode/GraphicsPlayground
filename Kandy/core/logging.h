#pragma once

#include <assert.h>
#include <stdarg.h>

#define LOG_TO_STR_HELPER(v) #v
#define LOG_TO_STR(v) LOG_TO_STR_HELPER(v)
#define LOG(msg, ...) Kandy::Core::Logger::Log(__FUNCTION__ ":" LOG_TO_STR(__LINE__) ":" msg, __VA_ARGS__)

#define ASSERT(cond) \
  do \
  { \
    if (!(cond)) \
    { \
      LOG("ASSERT: %s\n", #cond); \
      assert(false); \
    } \
  } while (0)

namespace Kandy
{
  namespace Core
  {
    namespace Logger
    {
      void Log(const char* const fmt, ...);
    }
  }
}
